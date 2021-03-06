static char args_doc[] = "";
static char doc[] = "MQTT SUBSCRIBER PROGRAM";
#include "struct.h"
static struct argp_option options[] = {
    {"use_tls", 't', "TLS", 0, "Use tls"},
    {"brokerIp", 'b', "BROKER", 0, "Broker IP adress"},
    {"brokerPort", 'p', "PORT", 0, "Broker port adress"},
    {"username", 'u', "USERNAME", 0, "Login to broker username"},
    {"password", 's', "PASSWORD", 0, "Login to broke password"},
    {"certificate", 'c', "CERTIFICATE", 0, "Certificate to use TLS"},
    {0}};


error_t parse_opt(int key, char *arg, struct argp_state *state);

void arguments_init( struct arguments *arguments);
