#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void addToList(struct topic **list, struct topic *topic){
    struct topic* temp = *list;
    if (temp == NULL) {
        *list = topic;
        return; 
    }

    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = topic;
}

void addToTopicEvent(struct event *event, struct topic *linked_topic){

    struct topic *temp = linked_topic;
    
    if (temp->event == NULL) {
        temp->event = event;
        return; 
    }

    while (temp->event->next != NULL) {
        temp->event = temp->event->next;
    }
    temp->event->next = event;
}

struct topic* createNode(char *topics, char *qos){

    struct topic *topic = NULL;
    topic = (struct topic*) malloc(sizeof(struct topic));
    if (topic == NULL) {
        return NULL;
    }
    strcpy(topic->topic, topics);
    strcpy(topic->qos, qos);
    topic->event = NULL;
    topic->next = NULL;
    return topic;
}

struct event* createNodeEvent(char *name, char *data_type, char *operator, char *value, char *sender, char *receiver, char *smtp, char *port, char *pass){

    struct event *event = NULL;
    event = (struct event*) malloc(sizeof(struct event));
    if (event == NULL) {
        return NULL;
    }
    strcpy(event->name, name);
    strcpy(event->type, data_type);
    strcpy(event->compare, operator);
    strcpy(event->value, value);
    strcpy(event->from_email, sender);
    strcpy(event->to_email, receiver);
    strcpy(event->smtp, smtp);
    strcpy(event->port, port);
    strcpy(event->pass, pass);
    event->timer = NULL;
    event->next = NULL;
    return event;
}

void printTopics(struct topic *topics){
	int i = 0;
    struct topic *temp = topics;
    
   while (temp != NULL) {
       printf("%s \n", temp->topic);
        //printf("Event %s   name %s\n", temp->topic, temp->event->name);

        while(temp->event != NULL){
            printf("Event key name %s\n", temp->event->name);
            temp->event = temp->event->next;
        }
    temp = temp->next;
    
    printf("\n");
   }
}