#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <stdlib.h>
#include "mail.h"
#include "struct.h"
#include <json-c/json.h>
#include "events.h"

int event_init(struct event *events, int *count)
{
  int rc = uci_load_events(&events, &count, 300);

  if (rc == 0){
    syslog(LOG_INFO, "Events readed successfully\n");
    return 0;
  }
  else{
    return 1;
  }
}
int dataTypeConvert(int typeNumber, char *typeChar)
{
  int res = 0;

  switch (typeNumber){
  case 0:
    strcpy(typeChar, "String");
    break;
  case 1:
    strcpy(typeChar, "Number");
    break;
  default:
    res = -1;
  }

  return res;
}

int compareTypeConvert(int typeNumber, char *typeChar)
{
  int res = 0;

  switch (typeNumber){
  case 0:
    strcpy(typeChar, "==");
    break;
  case 1:
    strcpy(typeChar, "<");
    break;
  case 2:
    strcpy(typeChar, ">");
    break;
  case 3:
    strcpy(typeChar, "!=");
    break;
  case 4:
    strcpy(typeChar, ">=");
    break;
  case 5:
    strcpy(typeChar, "<=");
    break;

  default:
    strcpy(typeChar, "Not found");
    res = -1;
  }

  return res;
}

int events_handler(char *topic, char *payload)
{

  struct event events[300];
  int tCount = -1;

  int rc = uci_load_events(events, &tCount, 300);
  if (rc != 0){
    syslog(LOG_INFO, "Events readed unsuccessfully\n");
    return 1;
  }

  json_object *jobj = json_tokener_parse(payload);
  json_object *tmp;

  for (int i = 0; i < tCount; i++){
    //If topic is found in events
    if (strcmp(topic, events[i].topic) == 0){
      int flag = -1;
      char *sender = events[i].from_email;
      char *password = events[i].pass;
      char *domain = events[i].smtp;
      char *port = events[i].port;
      char *sendTo = events[i].to_email;
      char payload_text[500];
      char details[250];
      char domainAndPort[100];
      sprintf(domainAndPort, "%s:%s", domain, port);
      char valueChar[20];
      dataTypeConvert(atoi(events[i].type), &valueChar);
      char operatorChar[20];
      compareTypeConvert(atoi(events[i].compare), &operatorChar);
      int rc = -1;

      if (json_object_object_get_ex(jobj, events[i].name, &tmp)){
        sprintf(details, "MQTT Topic: %s\n Value type: %s\n Compare: %s\n Expected value: %s\n Received value: %s\n Email: %s\n",
                events[i].topic, valueChar, operatorChar, events[i].value, json_object_get_string(tmp), events[i].to_email);

        sprintf(payload_text, "Subject: %s\r\n\r\n %s\r\n", "Event was triggered", &details);

        // If value is string then use strcmp
        if (strcmp(events[i].type, "0") == 0){
          syslog(LOG_INFO, "Event found\n");
          if (strcmp(json_object_get_string(tmp), events[i].value) == 0){
            syslog(LOG_INFO, "Event good\n");

            rc = send_mail(domainAndPort, sender, password, sendTo, payload_text);
          }
        }
        // If value is number use simple compare
        else if (strcmp(events[i].type, "1") == 0){
          int compare = atoi(events[i].compare);
    
          if (compare == EQUAL){
            if (atoi(events[i].value) == atoi(json_object_get_string(tmp))){
              flag = 0;
            }
          }
          else if (compare == LESS){
            if (atoi(json_object_get_string(tmp)) < atoi(events[i].value)){
              flag = 0;
            }
          }
          else if (compare == MORE){
            if (atoi(json_object_get_string(tmp)) > atoi(events[i].value)){
              flag = 0;
            }
          }
          else if (compare == NOT_EQUAL){
            if (atoi(json_object_get_string(tmp)) != atoi(events[i].value)){
              flag = 0;
            }
          }
          else if (compare == EQUAL_OR_MORE){
            if (atoi(json_object_get_string(tmp)) >= atoi(events[i].value)){
              flag = 0;
            }
          }
          else if (compare == EQUAL_OR_LESS){
            if (atoi(json_object_get_string(tmp)) <= atoi(events[i].value)){
              flag = 0;
            }
          }
          else{
            flag = -1;
          }
          if (flag != -1){
            rc = send_mail(domainAndPort, sender, password, sendTo, payload_text);
          }
        }
      }
    }
  }
  return 0;
}