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

	qsr_params.NPKF = 3500.0;
	qsr_params.SPKF = 500.0;
	qsr_params.THRESHOLD1 = qsr_params.NPKF + 0.25 * (qsr_params.SPKF - qsr_params.NPKF);
	qsr_params.THRESHOLD2 = 0.5 * qsr_params.THRESHOLD1;

		while (!feof(file)) {
			//filtering and finding data
			data = filterData(getNextData(file));

			//finding peak
			peakDetection(&qsr_params, data);


		}

//	peakDetection(&qsr_params); // Perform Peak Detection

		return 0;

	}

