#include "data_retrieve.h"
#include "data_processing.h"
#include "email_alert.h"
#include "log_data.h"
#include <json-c/json.h>
#include <stdio.h>

#define API_CALLS 10

int main(){
    char apiUrl[] ="api.openweathermap.org/data/2.5/weather?q=Karachi,pk&units=metric&APPID=b7c04f5f2a59d8733376caf3567c11cc";
    char rawDataFileName[] = "raw_data.txt";
    
    double totalTemperature = 0.0;

    for (int i = 0; i < API_CALLS; ++i) {
        
        data_retrieve(apiUrl, rawDataFileName);

        double currentTemperature = process_data(rawDataFileName);
       
        totalTemperature += currentTemperature;
    }

    
    double averageTemperature = totalTemperature / API_CALLS;
    printf("Average Temperature: %.2f°C\n", averageTemperature);
    
    double dew_point = process_humidity(rawDataFileName, averageTemperature);
   
    char email_body[256]; 
    
    
    if (averageTemperature > 35) {
        printf("High temperature detected. Current temperature (%.2f°C) exceeds the threshold.\n", averageTemperature);
        log_data("High temperature detected");
        snprintf(email_body, sizeof(email_body), "High temperature detected. Current temperature (%.2f°C) exceeds the maximum threshold.\n", averageTemperature);
        send_email(email_body);
    } else if (averageTemperature < 10) {
        printf("Low temperature detected. Current temperature (%.2f°C) is below freezing.\n", averageTemperature);
        log_data("Low temperature detected");
        snprintf(email_body, sizeof(email_body), "Low temperature detected. Current temperature (%.2f°C) is below freezing.\n", averageTemperature);
        send_email(email_body);
    } else {
        printf("Temperature within normal range.\n");
        log_data("Temperature within normal range.");
    }
    
    
    if (dew_point > 20) {
        printf("High dew point detected. Relative humidity is high, indicating moist air.\n");
        log_data("High dew point detected");
        snprintf(email_body, sizeof(email_body), "High dew point detected. Relative humidity is high, indicating moist air.\n");
        send_email(email_body);
    } else if (dew_point < 10) {
        printf("Low dew point detected. Relative humidity is low, indicating dry air.\n");
        log_data("Low dew point detected");
        snprintf(email_body, sizeof(email_body), "Low dew point detected. Relative humidity is low, indicating dry air.\n");
        send_email(email_body);
    } else {
        printf("Dew point within normal range.\n");
        log_data("Dew point within normal range");
    }

    return 0;
}
