#include "sensor.h"
#include "filters.h"
#include <stdio.h>
#include <stdlib.h>
#include "qrs.h"

// Main function for organizing the program execution.
// The functions and object predefined are just for inspiration.
// Please change orden,names arguments to fit your solution.

int main() {
	QRS_params qrs_params; // Instance of the made avaiable through: #include "qsr.h"
	FILE *file;                  // Pointer to a file object
	file = openfile("ECG.txt");
	int data, time = 0;

	qrs_params.NPKF = 4500.0;
	qrs_params.SPKF = 750.0;
	qrs_params.THRESHOLD1 = qrs_params.NPKF + 0.25 * (qrs_params.SPKF - qrs_params.NPKF);
	qrs_params.THRESHOLD2 = 0.5 * qrs_params.THRESHOLD1;

		while (!feof(file)) {
			//filtering and finding data
			data = filterData(getNextData(file));

		//	printf("%d\n", data);
			//finding peak
			int exit = peakDetection(&qrs_params, data);
			if (exit == 1) {
			//	printf("%d\n", time);
			} else if(exit == 2) {
				//printf("%d\n", time);
			}
			time ++;

		}

//	peakDetection(&qsr_params); // Perform Peak Detection

		return 0;

	}

