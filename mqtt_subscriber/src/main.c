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

#define maxArray 300
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
    struct topic topics[maxArray];
    struct event events[maxArray];
    int tCount = -1;
    
	arguments_init(&arguments);

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    int rc = uci_load_topics(topics, &tCount, maxArray);

    if (rc == 0){
        syslog(LOG_INFO, "Topics readed successfully\n");
    } else {
        ret = 1;
        goto end;
    }



    char *sender = "rutx10.sender@gmail.com";
    char *password = "dominykas1";
    char *domain = "smtp.gmail.com";
    char *sendTo = "b.dominykas@gmail.com";
    char *payload_text =  "Some event happened\r\n";


    //rc = send_mail(domain, sender, password, sendTo);



    mqttService(arguments, topics, tCount, &interrupt);

end:
    closelog();
    unlock(&fd);

	return ret;
}