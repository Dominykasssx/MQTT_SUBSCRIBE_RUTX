#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <uci.h>
#include <syslog.h>
#include "struct.h"

static struct uci_context *g_ctx;

int uci_get_option(const char* option_path, char **out_str)
{
    int rc = 0;

    struct uci_ptr ptr;
    char* name = strdup(option_path);

    if ((rc = uci_lookup_ptr(g_ctx, &ptr, name, true)) != UCI_OK)
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

    return rc;
}

int uci_read_topics(struct topic *topics, int *tCount, int *maxCount)
{
    int rc = 0;
    int count = 0;

    #define STRBUFFER_LEN 256
    char buffer[STRBUFFER_LEN];

    for (int i = 0; i < *maxCount; i++) {

        char* topic;
        char* qos;

        snprintf(buffer, STRBUFFER_LEN - 1, "%s[%d].%s", "mqtt_subscriber.@mqttsubscribe_topic", i, "topic");
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


        snprintf(buffer, STRBUFFER_LEN - 1, "%s[%d].%s", "mqtt_subscriber.@mqttsubscribe_topic", i, "qos");
        rc = uci_get_option(buffer, &qos);
        if (rc) {
            break;
        }
        syslog(LOG_INFO, "Topic assigned");
        // Assign values
        topics[count].topic = topic;
        topics[count].qos = atoi(qos);

        free(qos);

        count++;
    }

    *tCount = count;
    
    return rc;
}

