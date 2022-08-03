// 127.0.0.1:9001.

#include  <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/wait.h>

#include <ubox/uloop.h>
#include <fcgiapp.h>
//#include <fcgi_stdio.h>

// ufd is uloop_fd.
static struct uloop_fd fcgi_ufd;
static FCGX_Request g_request;

static int fcgi_uloop_init (FCGX_Request *request_ptr, struct uloop_fd *ufd_ptr);

static void handle_request (struct uloop_fd *ufd, unsigned int events);

static int test_fork (FCGX_Request *request);
static int test_open (FCGX_Request *request);

int main (int argc, char *argv[]) {
    int ret;

    // printf("[DEBUG](%d) Progress is start.\n",getpid());

    /* initialize uloop. */
    uloop_inait();

    /* Initialize a fastcgi fd. */
    ret = fcgi_uloop_init(&g_request, &fcgi_ufd);
    if(ret) {
        // printf("[ERROR](%d) fcgi initialize is error!\n",getpid());
        return -1;
    }
    
    // printf("[DEBUG](%d) Service is running ...\n",getpid());
    
    uloop_run();

    uloop_done();

    return 0;
}

static int test_open (FCGX_Request *request) {
    int fd;
    char buffer[1024] = {0};
    int len;

    fd = open("/home/data/www/index.html", O_RDONLY);
    if ( fd < 0 ) {
        return -1;
    }

    FCGX_FPrintF(request->out,"Content-Disposition:attachment; filename=index.html\r\n");
    FCGX_FPrintF(request->out,"\r\n");

//    while ( len=read(fd, buffer, sizeof(buffer)) >= 0 ) {
//        printf("[DEBUG] read %d bytes.\n", len);
//        printf("%s",buffer);
//        FCGX_PutStr(buffer, len, request->out);
//    }

    while( (len = read(fd, buffer, sizeof(buffer))) > 0) {
        FCGX_PutStr(buffer, len, request->out);
    }

    close(fd);
    return 0;
}

static int test_fork (FCGX_Request *request) {
    pid_t pid;
    int pipefd[2];
    char buffer[1024] = {0};
    int len;

    if( pipe(pipefd) ) {
        return -1;
    }

    pid = vfork();
    if (pid == -1 ) {
        return -1;
    } else if (pid == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], 1);
        close(pipefd[1]);

        execl("/usr/bin/cat", "/usr/bin/cat", "/home/data/www/test.txt", NULL);
    } else {
        close(pipefd[1]);

        FCGX_FPrintF(request-out, "Connection");
        FCGX_FPrintF(request->out, "Content-Disposition:attachment; filename=text.txt   \r\n");
        FCGX_FPrintF(request->out, "\r\n");

        while ((len=read(pipefd[0],buffer,sizeof(buffer))) > 0) {
            FCGX_PutStr(buffer, len, request->out);
        }

        waitpid(pid, NULL, 0);
        close(pipefd[0]);
        return 0;
    }
}

static void handle_request (struct uloop_fd *ufd, unsigned int events) {
    FCGX_Request *request = &g_request;
    char *value;

    /* Check events. */
    if ( !(events&ULOOP_READ) ) {
        printf("[DEBUG](%d) event not ULOOP_READ\n", getpid());
        return;
    }

    FCGX_Accept_r(request);

    printf("[DEBUG](%d) Find a %s request from %s:%s\n", getpid(),
           FCGX_GetParam("REQUEST_METHOD",request->envp),
           FCGX_GetParam("REMOTE_ADDR",request->envp),
           FCGX_GetParam("REMOTE_PORT",request->envp));

    value = FCGX_GetParam("QUERY_STRING",request->envp);
    if ( value == NULL ) {
        printf("search content: %s\n", value);
    }

    test_fork(request);

    FCGX_Finish_r(request);
    return;
}


static int fcgi_uloop_init (FCGX_Request *request_ptr, struct uloop_fd *ufd_ptr) {
    int sockfd;

    FCGX_Init();

    sockfd = FCGX_OpenSocket("127.0.0.1:9001", 20);
    if ( sockfd < 0 )
        return -1;

    FCGX_InitRequest(request_ptr, sockfd, 0);
    
    ufd_ptr->cb = handle_request;
    ufd_ptr->fd = sockfd;

    uloop_fd_add(ufd_ptr, ULOOP_READ);
    
    return 0;
}
