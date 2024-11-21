#ifndef DATA_PROCESSING_H
#define DATA_PROCESSING_H

double process_data(char *inputFileName);
void analyze_data(const char *json_data);
char *read_json_data(const char *inputFileName);
double process_humidity(const char *inputFileName, double averageTemperature);

#endif
