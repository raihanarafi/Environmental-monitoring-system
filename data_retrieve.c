#include "data_retrieve.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

void data_retrieve(char *url, char *outputFileName) {
    CURL *curl;
    FILE *outputFile;
    
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        outputFile = fopen(outputFileName, "w");
        if (outputFile) {
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, outputFile);
            CURLcode res = curl_easy_perform(curl);
            fclose(outputFile);
            curl_easy_cleanup(curl);
            
            if (res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            } else {
                printf("Data fetched successfully.\n");
            }
        } else {
            fprintf(stderr, "Error opening file for writing.\n");
        }
    }
    
    curl_global_cleanup();
}
