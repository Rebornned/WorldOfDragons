#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

void send_email(char *recept, char *code);
char dest[200];
char name_dest[] = "miltonsalgadoleandro@gmail.com";

static const char *payload_text[] = {
  "Date: Mon, 29 Nov 2021 21:54:29 +1100\r\n",
  dest,
  "From: rebornned.org@gmail.com\r\n",
  "Subject: Recuperação de senha\r\n",
  "\r\n", /* empty line to divide headers from body, see RFC5322 */
  "Responda este email com número do CPF e cartão. Este email foi enviado por um código C.\r\n",
  NULL
};

struct upload_status {
  int lines_read;
};

static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
{
  struct upload_status *upload_ctx = (struct upload_status *)userp;
  const char *data;

  if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
    return 0;
  }

  data = payload_text[upload_ctx->lines_read];

  if(data) {
    size_t len = strlen(data);
    memcpy(ptr, data, len);
    upload_ctx->lines_read++;
    return len;
  }

  return 0;
}

void send_email(char *recept, char *code) {
  CURL *curl;
  CURLcode res = CURLE_OK;
  struct curl_slist *recipients = NULL;
  struct upload_status upload_ctx = { 0 };

  // Atualiza o payload_text com os dados fornecidos
  sprintf(dest, "To: %s\r\n", recept);

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_USERNAME, "rebornned.org@gmail.com");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "fqbv yvly oaqa qbiw");
    curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");

    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);  // Usar SSL/TLS
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, "rebornned.org@gmail.com");
    
    recipients = curl_slist_append(recipients, recept);
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    /* Para depuração: */
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    res = curl_easy_perform(curl);

    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

    curl_slist_free_all(recipients);
    curl_easy_cleanup(curl);
  }
}

int main() {
    send_email("dhemersonsousacoc@gmail.com", "12345");
    return 0;
}
