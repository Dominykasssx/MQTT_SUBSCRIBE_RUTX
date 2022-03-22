#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <uci.h>
#include <syslog.h>
#include "struct.h"
#include <string.h>

static const char *delimiter = " ";

int uci_get_option(const char* option_path, char **out_str)
{
    int rc = 0;

    struct uci_context *c;
    struct uci_ptr ptr;
    c = uci_alloc_context ();

    char* name = strdup(option_path);

    if ((rc = uci_lookup_ptr(c, &ptr, name, true)) != UCI_OK)
    {
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

int uci_load_topics(struct topic *topics, int *tCount, int maxTopics)
{
    int rc = 0;
    int count = 0;

    char buffer[255];

    for (int i = 0; i < maxTopics; i++) {

        char* topic;
        char* qos;

        snprintf(buffer, 255, "%s[%d].%s", "mqtt_subscriber.@topic", i, "topic");
        rc = uci_get_option(buffer, &topic);
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
        rc = uci_get_option(buffer, &qos);
        if (rc) {
            break;
        }

        topics[count].topic = topic;
        topics[count].qos = atoi(qos);

        count++;
    }

    *tCount = count;
    
    return rc;
}

int uci_load_events(struct event *events, int *tCount, int maxEvents)
{
    int rc = 0;
    int count = 0;

    char buffer[255];

    for (int i = 0; i < maxEvents; i++) {
        char* topic;
        char* name;
        char* data_type;
        char* value;
        char* operator;
        char* sender;
        char* receiver;
        char* smtp;
        char* port;
        char* pass;
        //Reading topic
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
        //Reading name
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
        //Reading type
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
        //Reading value
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
        //Reading compare operator
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
        //Reading from email
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
        //Reading to email
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

        //Reading smtp
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

        //Reading port
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

        //Reading password
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

        
        
        events[count].topic = topic;
        events[count].name = name;
        events[count].type = data_type;
        events[count].compare = operator;
        events[count].value = value;
        events[count].from_email = sender;
        events[count].to_email = receiver;
        events[count].smtp = smtp;
        events[count].port = port;
        events[count].pass = pass;

        count++;
    }

    *tCount = count;
    
    return rc;
}