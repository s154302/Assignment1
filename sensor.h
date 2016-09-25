#ifndef SENSOR_H
#define SENSOR_H
#include <stdio.h>
#include <stdlib.h>

// Header file for the sensor functionality
// You are free to change and add functions 
// as much as you like

FILE* openfile(const char* filename);

int getNextData(FILE *file);

void write_to_file(const char* filename, int data);

void closefile(const char* filename);

#endif // SENSOR_H
