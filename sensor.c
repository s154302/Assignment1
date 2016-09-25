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
		printf("Error opening file!\nCreating new file: %s", filename);
		file = fopen(filename, "wb");
		exit(1);
	}

	return file;
}

void write_to_file(const char* filename, int data){
	FILE *file = fopen(filename, "w");
	if(file == NULL){
		printf("Error opening file!\nCreating new file: %s", filename);
		file = fopen(filename, "wb");
		exit(1);
	}
	fprintf(file, "%d\n", data);
	//printf("%s: %d\n", filename, data);

}

void closefile(const char* filename){
	fclose(filename);
}
