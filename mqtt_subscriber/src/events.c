#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <stdlib.h>
#include "mail.h"
#include "struct.h"
#include <json-c/json.h>
#include "events.h"
#include "uci_load.h"
#include <time.h>

int dataTypeConvert(int typeNumber, char *typeChar)
{
  int res = 0;

  switch (typeNumber)
  {
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

  switch (typeNumber)
  {
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
void getTime(char *buffer)
{
    time_t timer;
    struct tm* tm_info;
    timer = time(NULL);
    tm_info = localtime(&timer);

    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
}

void formEmail(struct topic *topic, struct event *event, char *reValue, char *payload_text)
{
  char valueChar[20];
  char operatorChar[20];
  char time[26];
  dataTypeConvert(atoi(event->type), valueChar);
  compareTypeConvert(atoi(event->compare), operatorChar);
  getTime(&time);

  sprintf(payload_text, "Subject: %s\r\n\r\n"
                        "MQTT topics: %s\n"
                        "Value type: %s\n" 
                        "Compare: %s\n"
                        "Expected value: %s\n"
                        "Received value: %s\n"
                        "Email: %s\n"
                        "Time: %s", "Event was triggered", topic->topic, valueChar, 
                        operatorChar, event->value, reValue, event->to_email, time);

  saveEmail(payload_text, time);

}

int events_handler(struct topic *topics, char *payload, char *topic)
{
  json_object *jobj = json_tokener_parse(payload);
  json_object *tmp;

  int flag = -1;
  char payload_text[500];
  int topicFoundFlag = -1;

  int rc = -1;
  while (topics != NULL)
  {
    if (strcmp(topics->topic, topic) == 0)
    {
      struct event *tmpEvent = topics->event;
      while (tmpEvent != NULL)
      {
        if (json_object_object_get_ex(jobj, &tmpEvent->name, &tmp))
        {
          if (strcmp(tmpEvent->type, "0") == 0)
          {
            if (strcmp(json_object_get_string(tmp), tmpEvent->value) == 0) {

              char domainAndPort[100];
              sprintf(domainAndPort, "%s:%s", tmpEvent->smtp, tmpEvent->port);

              
              formEmail(topics, tmpEvent, json_object_get_string(tmp), &payload_text);
              rc = send_mail(domainAndPort, tmpEvent->from_email, tmpEvent->pass, tmpEvent->to_email, payload_text);
            }
          }
          else if (strcmp(tmpEvent->type, "1") == 0)
          {
            int compare = atoi(tmpEvent->compare);

            if (compare == EQUAL) {
              if (atoi(tmpEvent->value) == atoi(json_object_get_string(tmp)))
              {
                flag = 0;
              }
            }
            else if (compare == LESS) {
              if (atoi(json_object_get_string(tmp)) < atoi(tmpEvent->value))
              {
                flag = 0;
              }
            }
            else if (compare == MORE) {
              if (atoi(json_object_get_string(tmp)) > atoi(tmpEvent->value))
              {
                flag = 0;
              }
            }
            else if (compare == NOT_EQUAL) {
              if (atoi(json_object_get_string(tmp)) != atoi(tmpEvent->value))
              {
                flag = 0;
              }
            }
            else if (compare == EQUAL_OR_MORE) {
              if (atoi(json_object_get_string(tmp)) >= atoi(tmpEvent->value))
              {
                flag = 0;
              }
            }
            else if (compare == EQUAL_OR_LESS) {
              if (atoi(json_object_get_string(tmp)) <= atoi(tmpEvent->value))
              {
                flag = 0;
              }
            }
            else
            {
              flag = -1;
            }
            if (flag != -1) {
              char domainAndPort[100];
              sprintf(domainAndPort, "%s:%s", tmpEvent->smtp, tmpEvent->port);
              formEmail(topics, tmpEvent, json_object_get_string(tmp), &payload_text);
              rc = send_mail(domainAndPort, tmpEvent->from_email, tmpEvent->pass, tmpEvent->to_email, payload_text);
              flag = -1;
            }
          }
        }
        tmpEvent = tmpEvent->next;
      }
    }
    topics = topics->next;
  }
  return 0;
}