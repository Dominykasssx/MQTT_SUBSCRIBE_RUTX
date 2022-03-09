struct arguments
{
    char *brokerIp;
    int brokerPort, use_tls;
};

struct topic
{
    char *topic;
    int qos;
};