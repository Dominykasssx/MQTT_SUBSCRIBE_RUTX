#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <stdlib.h>
#include "mail.h"
#include "struct.h"
 
int event_init(struct event *events, int *count)
{

      int rc= uci_load_events(&events, &count, 300);

    if (rc == 0){
        syslog(LOG_INFO, "Events readed successfully\n");
        return 0;
    } else {
        return 1;
    }

}
  int formater()
  {
    
  }
 
int events_handler(char *topic, char *payload)
{
    struct event events[300];
    int tCount = -1;
    
    int rc= uci_load_events(events, &tCount, 300);

    if (rc != 0){
        syslog(LOG_INFO, "Events readed unsuccessfully\n");
        return 1;
    }

    for (int i = 0; i < tCount; i++){
      if (strcmp(topic,events[i].topic) == 0){
        fprintf(stdout, "%s", "TOPIC FOUND");
        //If value is string then use strcmp
        if(strcmp(events[i].type, "string") == 0){
          if(strcmp(payload,events[i].value) == 0){
                        
            char *sender = events[i].from_email;
            char *password = "dominykas1";
            char *domain = "smtp.gmail.com";
            char *sendTo = events[i].to_email;

            char payload_text[1000];
            char details[500];

            sprintf(details,"MQTT Topic: %s\n Value type: %s\n Compare: %d\n Expected value: %s\n Received value: %s\n Email:%s\n",
                            events[i].topic, events[i].type, events[i].compare, events[i].value, payload, events[i].to_email);

            fprintf(stdout, "%s", details);
            sprintf(payload_text, "Subject: %s\r\n\r\n %s\r\n","Event was triggered", &details);

            int rc = -1;

            rc = send_mail(domain, sender, password, sendTo,payload_text);
          }
        }
        //If value is number use simple compare 
        else if(strcmp(events[i].type, "int") == 0){
          if (atoi(events[i].value) == atoi(payload))
          {
            char *sender = events[i].from_email;
            char *password = "dominykas1";
            char *domain = "smtp.gmail.com";
            char *sendTo = events[i].to_email;

            char payload_text[1000];
            char details[500];

            sprintf(details,"MQTT Topic: %s\n Value type: %s\n Compare: %d\n Expected value: %s\n Received value: %s\n Email:%s\n",
                            events[i].topic, events[i].type, events[i].compare, events[i].value, payload, events[i].to_email);

            fprintf(stdout, "%s", details);
            sprintf(payload_text, "Subject: %s\r\n\r\n %s\r\n","Event was triggered", &details);

            int rc = -1;

            rc = send_mail(domain, sender, password, sendTo,payload_text);

          }
        }
      }
    }
  return 0;
}