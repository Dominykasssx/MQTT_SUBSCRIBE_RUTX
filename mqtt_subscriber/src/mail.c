#include <stdio.h>
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
    /* Set username and password */
    curl_easy_setopt(curl, CURLOPT_USERNAME, fromEmail);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, fromPass);
    /* This is the URL for your mailserver. Note the use of port 587 here,
     * instead of the normal SMTP port (25). Port 587 is commonly used for
     * secure mail submission (see RFC4403), but you should use whatever
     * matches your server configuration. */
    curl_easy_setopt(curl, CURLOPT_URL, smtp);
 
    /* In this example, we will start with a plain text connection, and upgrade
     * to Transport Layer Security (TLS) using the STARTTLS command. Be careful
     * of using CURLUSESSL_TRY here, because if TLS upgrade fails, the transfer
     * will continue anyway - see the security discussion in the libcurl
     * tutorial for more details. */
    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
 
    /* If your server does not have a valid certificate, then you can disable
     * part of the Transport Layer Security protection by setting the
     * CURLOPT_SSL_VERIFYPEER and CURLOPT_SSL_VERIFYHOST options to 0 (false).
     *   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
     *   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
     * That is, in general, a bad idea. It is still better than sending your
     * authentication details in plain text though.  Instead, you should get
     * the issuer certificate (or the host certificate if the certificate is
     * self-signed) and add it to the set of certificates that are known to
     * libcurl using CURLOPT_CAINFO and/or CURLOPT_CAPATH. See docs/SSLCERTS
     * for more information. */
    //curl_easy_setopt(curl, CURLOPT_CAINFO, "/path/to/certificate.pem");
 
    /* Note that this option is not strictly required, omitting it will result
     * in libcurl sending the MAIL FROM command with empty sender data. All
     * autoresponses should have an empty reverse-path, and should be directed
     * to the address in the reverse-path which triggered them. Otherwise,
     * they could cause an endless loop. See RFC 5321 Section 4.5.5 for more
     * details.
     */
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, fromEmail);
 
    /* Add two recipients, in this particular case they correspond to the
     * To: and Cc: addressees in the header, but they could be any kind of
     * recipient. */
    recipients = curl_slist_append(recipients, toEmail);
    //recipients = curl_slist_append(recipients, CC_MAIL);
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
 
    /* We are using a callback function to specify the payload (the headers and
     * body of the message). You could just use the CURLOPT_READDATA option to
     * specify a FILE pointer to read from. */
    payload_text = malloc(sizeof(char) * strlen(payload)+1);
    strcpy(payload_text, payload);

    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
 
    /* Since the traffic will be encrypted, it is very useful to turn on debug
     * information within libcurl to see what is happening during the transfer.
     */
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
 
    /* Send the message */
    res = curl_easy_perform(curl);


    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* Free the list of recipients */
    curl_slist_free_all(recipients);
    free(payload_text);
    /* Always cleanup */
    curl_easy_cleanup(curl);
  }
 
  return (int)res;
}