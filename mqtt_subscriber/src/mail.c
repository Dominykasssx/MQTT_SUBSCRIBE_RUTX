#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
  
char *payload_text;
 
struct upload_status {
  size_t bytes_read;
};
 
static size_t payload_source(char *ptr, size_t size, size_t nmemb, void *userp)
{
  struct upload_status *upload_ctx = (struct upload_status *)userp;
  
  const char *data;
  size_t room = size * nmemb;
 
  if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
    return 0;
  }
  data = &payload_text[upload_ctx->bytes_read];
 
  if(data) {
    size_t len = strlen(data);
    if(room < len)
      len = room;
    memcpy(ptr, data, len);
    upload_ctx->bytes_read += len;
 
    return len;
  }
  return 0;
}
 
int send_mail(char *smtp, char *fromEmail, char *fromPass, char *toEmail, char *payload)
{
  CURL *curl;
  CURLcode res = CURLE_OK;
  struct curl_slist *recipients = NULL;
  struct upload_status upload_ctx = { 0 };
 
  curl = curl_easy_init();
  if(curl) {

    curl_easy_setopt(curl, CURLOPT_USERNAME, fromEmail);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, fromPass);
    curl_easy_setopt(curl, CURLOPT_URL, smtp);
    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, fromEmail);
    recipients = curl_slist_append(recipients, toEmail);
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
 
    payload_text = malloc(sizeof(char) * strlen(payload)+1);
    strcpy(payload_text, payload);

    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
 
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);
 
    /* Send the message */
    res = curl_easy_perform(curl);


    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
 
    /* Free the list of recipients */
    curl_slist_free_all(recipients);
    free(payload_text);
    /* Always cleanup */
    curl_easy_cleanup(curl);
  }
 
  return (int)res;
}