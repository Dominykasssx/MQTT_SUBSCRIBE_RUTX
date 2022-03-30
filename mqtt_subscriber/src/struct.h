#include <time.h>

struct arguments
{
    char *brokerIp, *username, *password, *certificate;
    int brokerPort, use_tls;
};


struct topic
{
    char topic[125];
    char qos[2];
    struct event *event;
    struct topic *next;
};

struct event
{
    char name[200];
    char type[2];
    char compare[2];
    char value[200];
    char from_email[200];
    char to_email[200];
    char smtp[50];
    char port[10];
    char pass[200];
    time_t timer;
    struct event *next;
};
