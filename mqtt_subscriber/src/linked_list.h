void addToList(struct topic **list, struct topic *topic);
void addToTopicEvent(struct event *event, struct topic *linked_topic);
struct topic* createNode(char *topics, char *qos);
struct event* createNodeEvent(char *name, char *data_type, char *operator, char *value, char *sender, char *receiver, char *smtp, char *port, char *pass);
void printTopics(struct topic *topics);