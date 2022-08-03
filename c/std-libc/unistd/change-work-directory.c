#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <glob.h>


int main(int argc, char *argv)
{
	int i, fd;
	char buffer[1024];
	glob_t g;

	getcwd(buffer, sizeof(buffer));
	printf("Current work directory: %s\n", buffer);

	chdir("/tmp");
	getcwd(buffer, sizeof(buffer));
	printf("Current work directory: %s\n", buffer);


	fd = open("/usr/include", O_RDONLY | O_DIRECTORY);
	fchdir(fd);
	getcwd(buffer, sizeof(buffer));
	printf("Current work directory: %s\n", buffer);
	close(fd);

	glob("./*.h", 0, NULL, &g);
	for (i = 0; i < g.gl_pathc; i++)
		printf("path %s\n", g.gl_pathv[i]);



	return 0;
}