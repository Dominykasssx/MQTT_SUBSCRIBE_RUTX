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