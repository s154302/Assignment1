#include "sensor.h"
#include <stdio.h>
#include <stdlib.h>

int getNextData(FILE *file) {
	int value;
	fscanf(file, "%d", &value);
	return value; // return sensor value
}

FILE* openfile(const char* filename) {
	FILE *file = fopen(filename, "r");
	if(file == NULL){
		printf("Error opening %s!", filename);
		exit(1);
	}

	return file;
}

FILE* openWritingfile(const char* filename) {
	FILE *file = fopen(filename, "w");
		if(file == NULL){
			printf("Error opening file!\nCreating new file: %s", filename);
			file = fopen(filename, "wb");
			exit(1);
		}

	return file;
}

