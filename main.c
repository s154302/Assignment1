#include "sensor.h"
#include "filters.h"
#include <stdio.h>
#include <stdlib.h>
#include "qrs.h"
#include <time.h>

// Main function for organizing the program execution.
// The functions and object predefined are just for inspiration.
// Please change orden,names arguments to fit your solution.

int main(int argc, char *argv[]) {
	//Time anaylysis of program
	clock_t start, end;
	double cpu_time_used;
	start = clock();


	QRS_params qrs_params; // Instance of the made avaiable through: #include "qsr.h"
	FILE *file, *Rpeaks, *Rpeaks_time, *SB_Rpeaks, *SB_Rpeaks_time, *threshold1,
			*threshold2, *vECG;               // Pointer to a file object
	file = openfile("ECG.txt");
	Rpeaks = openWritingfile("Rpeaks.txt");
	Rpeaks_time = openWritingfile("Rpeaks_time.txt");
	SB_Rpeaks = openWritingfile("SB_Rpeaks.txt");
	SB_Rpeaks_time = openWritingfile("SB_Rpeaks_time.txt");
	threshold1 = openWritingfile("Threshold1.txt");
	threshold2 = openWritingfile("Threshold2.txt");
	vECG = openWritingfile("vECG.txt");

	int data = 0, previous_time1 = 0, previous_time2 = 0,
			sampleCounter = 0, pulse = 0, exit = 0;

	qrs_params.NPKF = 4000;
	qrs_params.SPKF = 1500;
	qrs_params.THRESHOLD1 = qrs_params.NPKF
			+ 0.25 * (qrs_params.SPKF - qrs_params.NPKF);
	qrs_params.THRESHOLD2 = 0.5 * qrs_params.THRESHOLD1;
	qrs_params.RR = 0;
	qrs_params.Rpeak = 2000;
	qrs_params.SB_Rpeak = 2000;
	qrs_params.Rpeak_time = 0;
	qrs_params.SB_Rpeak_time = 0;
	qrs_params.seconds = 0;

	while (!feof(file)) {
		//filtering and finding data
		data = filterData(getNextData(file));

		//finding peak
		exit = peakDetection(&qrs_params, data);

		if (sampleCounter == 250) {
			sampleCounter = 1;
			qrs_params.seconds++;
		} else {
			sampleCounter++;
		}

		if (exit == 1) {

			// Warning
			if (qrs_params.Rpeak < 2000 || qrs_params.SB_Rpeak < 2000) {
				printf("Warning! Low peak at %d!\n", qrs_params.seconds);
			}

			// Rpeak
			if (qrs_params.Rpeak_time != previous_time1) {
				fprintf(Rpeaks, "%d\n", qrs_params.Rpeak);
				fprintf(Rpeaks_time, "%d\n", qrs_params.Rpeak_time);
			}

			// SB Rpeaks
			if (qrs_params.SB_Rpeak_time != previous_time2) {
				fprintf(SB_Rpeaks, "%d\n", qrs_params.SB_Rpeak);
				fprintf(SB_Rpeaks_time, "%d\n", qrs_params.SB_Rpeak_time);
			}

			previous_time1 = qrs_params.Rpeak_time;
			previous_time2 = qrs_params.SB_Rpeak_time;

			// Pulse
			pulse = 6000/((qrs_params.RR*100)/250);

			printf("Rpeak: %d\nTime: %d seconds\nPulse: %d\n\n", qrs_params.Rpeak, qrs_params.seconds, pulse);

		}

		// Thresholds
		fprintf(threshold1, "%d\n", qrs_params.THRESHOLD1);
		fprintf(threshold2, "%d\n", qrs_params.THRESHOLD2);

		// Data
		fprintf(vECG, "%d\n", data);

	}

	fclose(file);
	fclose(Rpeaks);
	fclose(Rpeaks_time);
	fclose(SB_Rpeaks);
	fclose(SB_Rpeaks_time);
	fclose(threshold1);
	fclose(threshold2);
	fclose(vECG);

	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("CPU time used: %d", cpu_time_used);
	return 0;

}

