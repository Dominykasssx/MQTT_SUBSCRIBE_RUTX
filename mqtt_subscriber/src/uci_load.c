#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uci.h>
#include "struct.h"
#include "linked_list.h"


int uci_get_option(const char* option_path, char **out_str)
{
    int rc = 0;

    struct uci_context *c;
    struct uci_ptr ptr;
    c = uci_alloc_context ();

    char* name = strdup(option_path);

    if ((rc = uci_lookup_ptr(c, &ptr, name, true)) != UCI_OK){
        fprintf(stderr, "Can not find anything in uci_lookup_ptr, section name = \"%s\"\n", name);
        goto free_ptr;
    }  

    if (ptr.o == NULL) {
        rc = 1;
        goto free_ptr;
    }

    switch(ptr.o->type) {

    case UCI_TYPE_STRING:
        *out_str = strdup(ptr.o->v.string);
        break;

    case UCI_TYPE_LIST:
        rc = 1;
        goto free_ptr;
  
    default:
        rc = 1;
        goto free_ptr;
    }
    free_ptr:
    free(name);
    uci_free_context (c);

    return rc;
}

void uci_load_topics(int max, struct topic **list)
{
   struct topic *topics = NULL;

    int rc = 0;
    int count = 0;
    char *tmpTopic;
    char *tmpQos;

    char buffer[255];

    for (int i = 0; i < max; i++) {
        snprintf(buffer, 255, "%s[%d].%s", "mqtt_subscriber.@topic", i, "topic");
        rc = uci_get_option(buffer, &tmpTopic);
        if (rc == 1) { 
            rc = 0;
            break;
        }
        else if (rc) {
            fprintf(stderr, "Error while setting uci topics\n");
            rc = 1;
            break;
        }
        snprintf(buffer, 255, "%s[%d].%s", "mqtt_subscriber.@topic", i, "qos");
        rc = uci_get_option(buffer, &tmpQos);
        if (rc == 1) { 
            rc = 0;
            break;
        }
        else if (rc) {
            fprintf(stderr, "Error while setting uci topics\n");
            rc = 1;
            break;
        }

        topics = createNode(tmpTopic, tmpQos);
        addToList(list, topics);
    }
}




void uci_load_events(int max, struct topic **list)
{
   struct event *events = NULL;

    int rc = 0;
    int count = 0;
    char *topic;
    char *name;
    char *data_type;
    char *value;
    char *operator;
    char *sender;
    char *receiver;
    char *smtp;
    char *port;
    char *pass;

    char buffer[255];

    for (int i = 0; i < max; i++) {
         snprintf(buffer, 255, "%s[%d].%s", "mqtt_subscriber.@event", i, "topic");
        rc = uci_get_option(buffer, &topic);
        if (rc == 1) { 
            rc = 0;
            break;
        }
        else if (rc) {
            fprintf(stderr, "Error while setting uci events\n");
            rc = 1;
            break;
        }
        snprintf(buffer, 255, "%s[%d].%s", "mqtt_subscriber.@event", i, "name");
        rc = uci_get_option(buffer, &name);
        if (rc == 1) { 
            rc = 0;
            break;
        }
        else if (rc) {
            fprintf(stderr, "Error while setting uci events\n");
            rc = 1;
            break;
        }
        snprintf(buffer, 255, "%s[%d].%s", "mqtt_subscriber.@event", i, "data_type");
        rc = uci_get_option(buffer, &data_type);
        if (rc == 1) { 
            data_type = "0";
            rc = 0;
            
        }
        else if (rc) {
            fprintf(stderr, "Error while setting uci events\n");
            rc = 1;
            break;
        }
        snprintf(buffer, 255, "%s[%d].%s", "mqtt_subscriber.@event", i, "value");
        rc = uci_get_option(buffer, &value);
        if (rc == 1) { 
            rc = 0;
            break;
        }
        else if (rc) {
            fprintf(stderr, "Error while setting uci events\n");
            rc = 1;
            break;
        }
        snprintf(buffer, 255, "%s[%d].%s", "mqtt_subscriber.@event", i, "operator");
        rc = uci_get_option(buffer, &operator);
        if (rc == 1) { 
            operator = "0";
            rc = 0;
        }
        else if (rc) {
            fprintf(stderr, "Error while setting uci events\n");
            rc = 1;
            break;
        }
        snprintf(buffer, 255, "%s[%d].%s", "mqtt_subscriber.@event", i, "sender_conf");
        rc = uci_get_option(buffer, &sender);
        if (rc == 1) { 
            rc = 0;
            break;
        }
        else if (rc) {
            fprintf(stderr, "Error while setting uci events\n");
            rc = 1;
            break;
        }
        snprintf(buffer, 255, "%s[%d].%s", "mqtt_subscriber.@event", i, "receiver_mail");
        rc = uci_get_option(buffer, &receiver);
        if (rc == 1) { 
            rc = 0;
            break;
        }
        else if (rc) {
            fprintf(stderr, "Error while setting uci events\n");
            rc = 1;
            break;
        }

        snprintf(buffer, 255, "%s[%d].%s", "mqtt_subscriber.@event", i, "smtp");
        rc = uci_get_option(buffer, &smtp);
        if (rc == 1) { 
            rc = 0;
            break;
        }
        else if (rc) {
            fprintf(stderr, "Error while setting uci events\n");
            rc = 1;
            break;
        }

        snprintf(buffer, 255, "%s[%d].%s", "mqtt_subscriber.@event", i, "smtp_port");
        rc = uci_get_option(buffer, &port);
        if (rc == 1) { 
            rc = 0;
            break;
        }
        else if (rc) {
            fprintf(stderr, "Error while setting uci events\n");
            rc = 1;
            break;
        }

        snprintf(buffer, 255, "%s[%d].%s", "mqtt_subscriber.@event", i, "sender_pass");
        rc = uci_get_option(buffer, &pass);
        if (rc == 1) { 
            rc = 0;
            break;
        }
        else if (rc) {
            fprintf(stderr, "Error while setting uci events\n");
            rc = 1;
            break;
        }
        
        events = createNodeEvent(name, data_type, operator, value, sender, receiver, smtp, port, pass);

        struct topic *tmp = *list;
        while (tmp != NULL){
            if (strcmp(topic, tmp->topic) == 0){
                addToTopicEvent(events, tmp);
            }
            tmp = tmp->next;
        }
    }
}
