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
	int data = 0, time = 0, previous_time1 = 0, previous_time2 = 0;

	qrs_params.NPKF = 4500;
	qrs_params.SPKF = 1500;
	qrs_params.THRESHOLD1 = qrs_params.NPKF + 0.25 * (qrs_params.SPKF - qrs_params.NPKF);
	qrs_params.THRESHOLD2 = 0.5 * qrs_params.THRESHOLD1;
	qrs_params.RR = 0;
	qrs_params.Rpeak = 0;
	qrs_params.SB_Rpeak = 0;
	qrs_params.Rpeak_time = 0;
	qrs_params.SB_Rpeak_time = 0;

		while (!feof(file)) {
			//filtering and finding data
			data = filterData(getNextData(file));

			//finding peak
			peakDetection(&qrs_params, data);

			//print params
			if(qrs_params.SB_Rpeak_time != previous_time2){
				// Thresholds
				//printf("%d\n", qrs_params.THRESHOLD1);
				//printf("%d\n",qrs_params.THRESHOLD2);

				// Rpeak
				//printf("%d\n", qrs_params.Rpeak);
				//printf("%d\n", qrs_params.Rpeak_time);

				// SB Rpeaks
				//printf("%d\n", qrs_params.SB_Rpeak);
				printf("%d\n", qrs_params.SB_Rpeak_time);
			}

			previous_time1 = qrs_params.Rpeak_time;
			previous_time2 = qrs_params.SB_Rpeak_time;

		}

		return 0;

	}

