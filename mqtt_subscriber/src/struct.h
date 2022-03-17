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
    int compare;
    char *value;
    char *from_email;
    char *to_email;
};