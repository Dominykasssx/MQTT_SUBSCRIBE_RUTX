struct arguments
{
    char *brokerIp, *username, *password, *certificate;
    int brokerPort, use_tls;
};

struct topic
{
    char *topic;
    int qos;
};

struct event
{
    char *topic;
    char *name;
    char *type;
    char *compare;
    char *value;
    char *from_email;
    char *to_email;
    char *smtp;
    char *port;
    char *pass;
};