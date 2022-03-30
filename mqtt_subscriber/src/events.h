#define EQUAL 0
#define LESS 1
#define MORE 2
#define NOT_EQUAL 3
#define EQUAL_OR_MORE 4
#define EQUAL_OR_LESS 5

int events_handler(struct topic *topics, char *payload, char *topic);