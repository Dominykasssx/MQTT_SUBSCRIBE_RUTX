struct arguments
{
    char *brokerIp, *username, *password;
    int brokerPort, use_tls;
};

struct topic
{
    char *topic;
    int qos;
};