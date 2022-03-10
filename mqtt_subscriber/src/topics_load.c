#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <uci.h>
#include <syslog.h>
#include "struct.h"
#include <string.h>

static const char *delimiter = " ";

static void uci_show_value(struct uci_option *o)
{
    struct uci_element *e;
    bool sep = false;

    switch(o->type) {
    case UCI_TYPE_STRING:
        printf("%s\n", o->v.string);
        break;
    case UCI_TYPE_LIST:
        uci_foreach_element(&o->v.list, e) {
            printf("%s%s", (sep ? delimiter : ""), e->name);
            sep = true;
        }
        printf("\n");
        break;
    default:
        printf("<unknown>\n");
        break;
    }
}

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
    //Method to show topics from uci
    //uci_show_value(ptr.o);

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

int uci_read_topics(struct topic *topics, int *tCount, int maxTopics)
{
    int rc = 0;
    int count = 0;

    char buffer[255];

    for (int i = 0; i < maxTopics; i++) {

        char* topic;
        char* qos;

        snprintf(buffer, 255, "%s[%d].%s", "mqtt_subscriber.@mqttsubscribe_topic", i, "topic");
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

        snprintf(buffer, 255, "%s[%d].%s", "mqtt_subscriber.@mqttsubscribe_topic", i, "qos");
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
