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
	int data, peak_determination[3], peaks[100];

//	while(!feof(file)) {
	for (int i = 0; i < 200; i++) {
		data = filterData(getNextData(file));
                printf("%d\n", data);
		for (int i = 2; i > 0; i--) {
			peak_determination[i] = peak_determination[i - 1];
		}
		peak_determination[0] = data;

		// Below needs revision.

		if(peak_determination[1] > peak_determination[0] && peak_determination[1] > peak_determination[2]) {
			for(int i = 99; i > 1; i--) {
				peaks[i] = peaks[i - 1];
			}
			peaks[0] = peak_determination[1];

			struct QRS_params peak;


		}


	}

//	peakDetection(&qsr_params); // Perform Peak Detection

	return 0;
}

