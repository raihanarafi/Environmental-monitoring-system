#include <stdio.h>
#include <json-c/json.h>
#include "data_processing.h"
#include <time.h>
#include <stdlib.h>
#include "data_retrieve.h"
#include "log_data.h"


typedef struct {
    double current_temperature; 
    double min_temperature;     
    double max_temperature;     
} TemperatureInfo;

TemperatureInfo extract_temperature(const char *json_data) {
    TemperatureInfo temperatureInfo = {0.0, 0.0, 0.0};

    struct json_object *root = json_tokener_parse(json_data);
    if (root == NULL) {
        fprintf(stderr, "JSON parsing error\n");
        return temperatureInfo;
    }

    struct json_object *main_json;
    if (json_object_object_get_ex(root, "main", &main_json)) {
        struct json_object *temperature_json;
        if (json_object_object_get_ex(main_json, "temp", &temperature_json)) {
            if (json_object_is_type(temperature_json, json_type_double)) {
                temperatureInfo.current_temperature = json_object_get_double(temperature_json);
            } else {
                fprintf(stderr, "Invalid temperature data in JSON.\n");
            }
        } else {
            fprintf(stderr, "Key 'temp' not found in 'main' JSON object.\n");
        }

        struct json_object *max_temp_json;
        struct json_object *min_temp_json;

        if (json_object_object_get_ex(main_json, "temp_min", &min_temp_json) &&
            json_object_object_get_ex(main_json, "temp_max", &max_temp_json)) {
            if (json_object_is_type(min_temp_json, json_type_double) &&
                json_object_is_type(max_temp_json, json_type_double)) {
                temperatureInfo.min_temperature = json_object_get_double(min_temp_json);
                temperatureInfo.max_temperature = json_object_get_double(max_temp_json);
            } else {
                fprintf(stderr, "Invalid min or max temperature data in JSON.\n");
            }
        } else {
            fprintf(stderr, "Key 'temp_min' or 'temp_max' not found in 'main' JSON object.\n");
        }
    } else {
        fprintf(stderr, "Key 'main' not found in JSON.\n");
    }

    json_object_put(root);
    return temperatureInfo;
}

void analyze_data(const char *json_data) {
    TemperatureInfo temperatureInfo = extract_temperature(json_data);

    if (temperatureInfo.current_temperature >= 0.0) {
        printf("Current Temperature (Celsius): %.2f°C\n", temperatureInfo.current_temperature);
        printf("Min Temperature (Celsius): %.2f°C\n", temperatureInfo.min_temperature);
        printf("Max Temperature (Celsius): %.2f°C\n", temperatureInfo.max_temperature);
    }   
}

double process_data(char *inputFileName) {
    FILE *inputFile = fopen(inputFileName, "r");
    double currentTemperature = 0.0;

    if (inputFile) {
        fseek(inputFile, 0, SEEK_END);
        long fileSize = ftell(inputFile);
        fseek(inputFile, 0, SEEK_SET);

        char *json_data = malloc(fileSize + 1);
        if (json_data) {
            fread(json_data, 1, fileSize, inputFile);
            fclose(inputFile);

            json_data[fileSize] = '\0'; 
            
            TemperatureInfo temperatureInfo = extract_temperature(json_data);

            if (temperatureInfo.current_temperature >= 0.0) {
                currentTemperature = temperatureInfo.current_temperature;
            } else {
                fprintf(stderr, "Failed to extract temperature data.\n");
            }

            analyze_data(json_data);

            free(json_data);
        } else {
            fprintf(stderr, "Memory allocation error.\n");
        }
    } else {
        fprintf(stderr, "Error opening file for reading.\n");
    }
    
    return currentTemperature;
}

typedef struct {
    double humidity; 
} HumidityInfo;

double extract_humidity(const char *json_data) {
    double humidity = -1.0; 

    struct json_object *root = json_tokener_parse(json_data);
    if (root == NULL) {
        fprintf(stderr, "JSON parsing error\n");
        return humidity;
    }

    struct json_object *main_json;
    if (json_object_object_get_ex(root, "main", &main_json)) {
        struct json_object *humidity_json;
        if (json_object_object_get_ex(main_json, "humidity", &humidity_json)) {
            if (json_object_is_type(humidity_json, json_type_int)) {
                humidity = json_object_get_int(humidity_json);
            } else {
                fprintf(stderr, "Invalid humidity data in JSON.\n");
            }
        } else {
            fprintf(stderr, "Key 'humidity' not found in 'main' JSON object.\n");
        }
    } else {
        fprintf(stderr, "Key 'main' not found in JSON.\n");
    }

    json_object_put(root);
    return humidity;
}

double calculate_dew_point(double temperature, double humidity) {
    return temperature - ((100 - humidity) / 5);
}

double process_humidity(const char *inputFileName, double averageTemperature) {
    char *json_data = read_json_data(inputFileName);
    if (json_data == NULL) {
        fprintf(stderr, "Error reading JSON data.\n");
        return 0;
    }

    double humidityInfo = extract_humidity(json_data);
    if (humidityInfo >= 0.0) {
        printf("Humidity: %.2f%%\n", humidityInfo);

        double temperature = averageTemperature;
        double dew_point = calculate_dew_point(temperature, humidityInfo);
        printf("Dew Point (Celsius): %.2f°C\n", dew_point);

        return dew_point;
    } else {
        fprintf(stderr, "Failed to extract humidity data.\n");
        log_data("Failed to extract humidity data");
    }
    
    return 0;
}

char *read_json_data(const char *inputFileName) {
    FILE *inputFile = fopen(inputFileName, "r");
    if (inputFile) {
        fseek(inputFile, 0, SEEK_END);
        long fileSize = ftell(inputFile);
        fseek(inputFile, 0, SEEK_SET);

        char *json_data = malloc(fileSize + 1);
        if (json_data) {
            fread(json_data, 1, fileSize, inputFile);
            fclose(inputFile);

            json_data[fileSize] = '\0';
            return json_data;
        } else {
            fprintf(stderr, "Memory allocation error.\n");
        }

        fclose(inputFile);
    } else {
        fprintf(stderr, "Error opening file for reading.\n");
    }

    return NULL;
}

