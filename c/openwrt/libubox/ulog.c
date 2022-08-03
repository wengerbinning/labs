#include <stdio.h>
#include <libubox/ulog.h>

/* Log Type - Copy from sys/syslog.h
 */
#define LOG_KERN         (0<<3)  /* kernel messages */
#define LOG_USER         (1<<3)  /* random user-level messages */
#define LOG_MAIL         (2<<3)  /* mail system */
#define LOG_DAEMON       (3<<3)  /* system daemons */
#define LOG_AUTH         (4<<3)  /* security/authorization messages */
#define LOG_SYSLOG       (5<<3)  /* messages generated internally by syslogd */
#define LOG_LPR          (6<<3)  /* line printer subsystem */
#define LOG_NEWS         (7<<3)  /* network news subsystem */
#define LOG_UUCP         (8<<3)  /* UUCP subsystem */
#define LOG_CRON         (9<<3)  /* clock daemon */
#define LOG_AUTHPRIV    (10<<3)  /* security/authorization messages (private) */
#define LOG_FTP         (11<<3)  /* ftp daemon */

/* Log Level - Copy form sys/syslog.h
 */
#define LOG_EMERG       0       /* system is unusable */
#define LOG_ALERT       1       /* action must be taken immediately */
#define LOG_CRIT        2       /* critical conditions */
#define LOG_ERR         3       /* error conditions */
#define LOG_WARNING     4       /* warning conditions */
#define LOG_NOTICE      5       /* normal but significant condition */
#define LOG_INFO        6       /* informational */
#define LOG_DEBUG       7       /* debug-level messages */

#define error(fmt, ...)     ulog(LOG_ERR,     fmt"\n", ## __VA_ARGS__)
#define warning(fmt, ...)   ulog(LOG_WARNING, fmt"\n", ## __VA_ARGS__)
#define notice(fmt, ...)    ulog(LOG_NOTICE,  fmt"\n", ## __VA_ARGS__)
#define info(fmt, ...)      ulog(LOG_INFO,    fmt"\n", ## __VA_ARGS__)
#define debug(fmt, ...)     ulog(LOG_DEBUG,   fmt"\n", ## __VA_ARGS__)

int main (int argc, char *argv[]) {

    ulog_open(ULOG_SYSLOG | ULOG_STDIO, LOG_DAEMON, "worker");

    error("Error Message");
    warning("Warning Message");
    notice("Notice Message");
    info("Normal Information");
    debug("Debug Message");

    ulog_close();
    return 0;
}




