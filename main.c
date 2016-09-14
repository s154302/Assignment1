#include "sensor.h"
#include "filters.h"
#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

// Main function for organizing the program execution.
// The functions and object predefined are just for inspiration.
// Please change orden,names arguments to fit your solution.

int main() {
	QRS_params qsr_params; // Instance of the made avaiable through: #include "qsr.h"
	FILE *file;                  // Pointer to a file object
	file = openfile("ECG.txt");
	int data;

	//    while(!feof(file)) {
	for (int i = 0; i < 200; i++) {
            data = filterData(data);
            
	}

	//peakDetection(&qsr_params); // Perform Peak Detection

	return 0;
}
