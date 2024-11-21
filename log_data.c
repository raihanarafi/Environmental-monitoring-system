#include <stdio.h>
#include <json-c/json.h>
#include <time.h>
#include <stdlib.h>
#include "log_data.h"

#define LOG_FILE "temperature_log.txt"

void log_data(const char *log_entry) {
    FILE *logFile = fopen(LOG_FILE, "a");  

    if (logFile) {
        time_t current_time;
        struct tm *time_info;
        char time_str[20];  

        time(&current_time);
        time_info = localtime(&current_time);
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", time_info);

        fprintf(logFile, "%s - %s\n", time_str, log_entry);
        fclose(logFile);
    } else {
        fprintf(stderr, "Error opening log file for writing.\n");
    }
}
