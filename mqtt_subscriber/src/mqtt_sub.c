#include <mosquitto.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "struct.h"
#include <syslog.h>
#include "logger.h"
#include "events.h"


/* Callback called when the client receives a CONNACK message from the broker. */
void on_connect(struct mosquitto *mosq, void *obj, int reason_code)
{
	if (reason_code == 0){
	fprintf(stdout, "Connection established\n");
	}
	else{
		fprintf(stdout, "Can't establish connection\n");
	}
}

/* Callback called when the broker sends a SUBACK in response to a SUBSCRIBE. */
static int subscribe(struct mosquitto *mosq, struct topic *topics, int topics_count)
{
    int rc = 0;
    for (int i = 0; i < topics_count; i++) {
        
        rc = mosquitto_subscribe(mosq, NULL, topics[i].topic, topics[i].qos);
        if (rc) {
            fprintf(stderr, "Failed to subscribe to topic \"%s\"\n", topics[i]);
            break;
        }
		else{
			fprintf(stdout, "Topic %s subscribed successfully\n", topics[i]);
		}
    }
    return rc;
}

/* Callback called when the client receives a message. */
void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{

	int rc1  = saveLog(msg->topic, (char *)msg->payload);

	if(rc1 == 1){
    	printf("Message was not logged\n");
    }
    else{
    	printf("Message logged\n");
    }
	fprintf(stdout, "From topic |%s| got message: |%s| qos: |%d|\n", msg->topic, (char *)msg->payload, msg->qos);
	events_handler(msg->topic, msg->payload);
}

int mqttService(struct arguments args, struct topic *topics, int tCount, int *interrupt)
{
	int ret = 0;
	int rc = openDatabase("Mqtt_subscriber");
    if (rc == 0){
    printf("OPENED DATABASE\n");
    }
    else{
    printf("ERROR OPENING DATABASE\n");
    ret = 1;
    }

	struct mosquitto *mosq;

	/* Required before calling other mosquitto functions */
	mosquitto_lib_init();

	/* Create a new client instance.
	 * id = NULL -> ask the broker to generate a client id for us
	 * clean session = true -> the broker should remove old sessions when we connect
	 * obj = NULL -> we aren't passing any of our private data for callbacks
	 */
	mosq = mosquitto_new(NULL, true, NULL);
	if(mosq == NULL){
		fprintf(stderr, "Error: Out of memory.\n");
		return 1;
	}
	if (strlen(args.certificate) != 0 || args.use_tls == 1){
		rc = mosquitto_tls_set(mosq, args.certificate, NULL, NULL, NULL, NULL);
		if (rc != 0){
			fprintf(stdout, "TLS not used\n");
			ret = 1;
		}
		else{
			fprintf(stdout, "TLS set successfully\n");
		}
	}
	else{
		fprintf(stdout, "TLS turned off / wrong certificate");
	}

	/* Configure callbacks. */
	mosquitto_connect_callback_set(mosq, on_connect);
	mosquitto_message_callback_set(mosq, on_message);

	    if (strlen(args.username) != 0 && strlen(args.password) != 0) {
        rc = mosquitto_username_pw_set(mosq, args.username, args.password);
        if (rc != MOSQ_ERR_SUCCESS) {
            fprintf(stderr, "Could not set username and password\n");
            ret = 1;
        }
		else{
			fprintf(stdout, "Successfully set username and password\n");
		}
    }
	rc = mosquitto_connect(mosq, args.brokerIp, args.brokerPort, 60);
	if(rc != MOSQ_ERR_SUCCESS){
		mosquitto_destroy(mosq);
		fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
		ret = 1;
	}

	subscribe(mosq, topics, tCount);


	mosquitto_loop_start(mosq);
	while (*interrupt == 0) {
        fflush(stdout);
    }
	if(*interrupt == 1){
		mosquitto_loop_stop(mosq, true);
		syslog(LOG_INFO, "MQTT connection lost");
		 rc =closeDatabase();
    	if (rc == 0){
    	fprintf(stdout, "CLOSED DATABASE\n");
    	}
    	else{
    	fprintf(stderr, "ERROR CLOSING DATABASE\n");
    	ret = 1;
    }
	}	
	mosquitto_lib_cleanup();
	return ret;


}