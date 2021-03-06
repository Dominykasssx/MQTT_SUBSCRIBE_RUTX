#include <stdio.h>
#include <signal.h>
#include <uci.h>
#include <stdlib.h>
#include <argp.h>
#include <memory.h>
#include <unistd.h>
#include <syslog.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/file.h>
#include "arguments_parse.h"
#include "mqtt_sub.h"
#include "uci_load.h"
#include "logger.h"
#include "mail.h"

#define maxList 300
#define LOCKFILE "/var/lock/mqtt_subscriber.lock"

int interrupt = 0;
char *configFilePath = NULL;
char *progname = "main";

int isLocked(int *fd)
{
    syslog(LOG_INFO, "Locking program");
    *fd = open(LOCKFILE, O_RDWR|O_CREAT);
    if (*fd < 0) {
        syslog(LOG_ERR, "can't open %s: %s", LOCKFILE, strerror(errno));
        return 1;
    }
     if (flock(*fd, LOCK_EX | LOCK_NB) < 0){
        syslog(LOG_ERR, "can't lock %s: %s", LOCKFILE, strerror(errno));
        return 1;
     }
    return 0;
}
void unlock(int *fd)
{
  syslog(LOG_INFO, "Unlocking program");
    if (flock(*fd, LOCK_UN) == -1){
        exit(1);
    }
    close(*fd);
}

void usage(void) 
{
    fprintf(stderr, "Usage: %s --config config_file_path\n", progname);
    exit(1);
}

void sigHandler(int signo)
{
    signal(SIGINT, NULL);
    fprintf(stdout, "Received signal: %d\n", signo);
    interrupt = 1;
}

static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char *argv[])
{
    int ret = 0;
    openlog("MQTT_SUBSCRIBE_PROGRAM",LOG_PID, LOG_USER);
    int fd;

    int rrc = isLocked(&fd);

    if (rrc != 0){
      syslog(LOG_ERR, "Program is already running from another daemon");
      ret = 0;
      goto end;
    }

    signal(SIGINT, sigHandler);
    signal(SIGTERM, sigHandler);

    struct arguments arguments;
    struct topic *list = NULL;
    
	arguments_init(&arguments);
    
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    printf("Started to read topics and events\n");

    uci_load_topics(maxList, &list);
    uci_load_events(maxList, &list);
    printf("Ended to read topics sand events\n");
    //printTopics(list);

    mqttService(arguments, list, &interrupt);

end:
    closelog();
    unlock(&fd);

	return ret;
}