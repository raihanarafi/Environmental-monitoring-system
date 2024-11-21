#include <stdio.h>
#include <string.h>
#include <curl/curl.h>


#define SMTP_SERVER "smtp://smtp.gmail.com:587"
#define USERNAME "cutiedai18@gmail.com" 
#define PASSWORD "pzil rulp uuev btol" 


#define TO_ADDRESS "raihanaraafi7@gmail.com"
#define SUBJECT "Temperature Alert"
#define BODY_INITIAL "Subject: Temperature Alert\r\n\n\n" 


struct EmailBodyInfo {
    const char *data;
    size_t size;
};


size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    struct EmailBodyInfo *email_body = (struct EmailBodyInfo *)userdata;
    size_t total_size = size * nmemb;

    if (email_body->size > 0) {
        size_t copy_size = (total_size < email_body->size) ? total_size : email_body->size;
        memcpy(ptr, email_body->data, copy_size);
        email_body->data += copy_size;
        email_body->size -= copy_size;
        return copy_size;
    } else {
        return 0; 
    }
}

void send_email(const char *body) {
    CURL *curl;
    CURLcode res;
    struct curl_slist *recipients = NULL;

    
    char email_body[4096];
    snprintf(email_body, sizeof(email_body), "%s%s", BODY_INITIAL, body);

    
    printf("libcurl version: %s\n", curl_version());

    
    curl_global_init(CURL_GLOBAL_DEFAULT);

   
    curl = curl_easy_init();
    if (curl) {

        
        curl_easy_setopt(curl, CURLOPT_URL, SMTP_SERVER);
        curl_easy_setopt(curl, CURLOPT_USERNAME, USERNAME);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, PASSWORD);

        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);

        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, USERNAME);
        recipients = curl_slist_append(recipients, TO_ADDRESS);
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        struct EmailBodyInfo email_body_info = {email_body, strlen(email_body)};
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
        curl_easy_setopt(curl, CURLOPT_READDATA, &email_body_info);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60L); 

        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        else
            printf("Email sent successfully!\n");

        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}
