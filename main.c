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
	FILE *file, *Rpeaks, *Rpeaks_time, *SB_Rpeaks, *SB_Rpeaks_time, *threshold1, *threshold2, *vECG;               // Pointer to a file object
	file = openfile("ECG.txt");
	Rpeaks = openWritingfile("Rpeaks.txt");
	Rpeaks_time = openWritingfile("Rpeaks_time.txt");
	SB_Rpeaks = openWritingfile("SB_Rpeaks.txt");
	SB_Rpeaks_time = openWritingfile("SB_Rpeaks_time.txt");
	threshold1 = openWritingfile("Threshold1.txt");
	threshold2 = openWritingfile("Threshold2.txt");
	vECG = openWritingfile("vECG.txt");


	int data = 0, time = 0, previous_time1 = 0, previous_time2 = 0;

	qrs_params.NPKF = 3000;
	qrs_params.SPKF = 500;
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
			vECG = fprintf(vECG, "%d\n", data);

			//finding peak
			peakDetection(&qrs_params, data);

			if(qrs_params.Rpeak_time != previous_time1){
				// Rpeak
				fprintf(Rpeaks, "%d\n", qrs_params.Rpeak);
				fprintf(Rpeaks_time, "%d\n", qrs_params.Rpeak_time);
				//write_to_file("Rpeaks.txt",qrs_params.Rpeak);
				//write_to_file("Rpeaks_time.txt",qrs_params.Rpeak_time);
			}

			if(qrs_params.SB_Rpeak_time != previous_time2){
				// SB Rpeaks
				fprintf(SB_Rpeaks, "%d\n", qrs_params.SB_Rpeak);
				fprintf(SB_Rpeaks_time, "%d\n", qrs_params.SB_Rpeak_time);
				//write_to_file("SB_Rpeaks.txt", qrs_params.SB_Rpeak);
				//write_to_file("SB_Rpeaks_time.txt", qrs_params.SB_Rpeak_time);
			}

			// Thresholds
			fprintf(threshold1, "%d\n", qrs_params.THRESHOLD1);
			fprintf(threshold2, "%d\n", qrs_params.THRESHOLD2);
			//write_to_file("threshold1.txt", qrs_params.THRESHOLD1);
			//write_to_file("threshold2.txt",qrs_params.THRESHOLD2);

			previous_time1 = qrs_params.Rpeak_time;
			previous_time2 = qrs_params.SB_Rpeak_time;

		}
		fclose(file);
		fclose(Rpeaks);
		fclose(Rpeaks_time);
		fclose(SB_Rpeaks);
		fclose(SB_Rpeaks_time);
		fclose(threshold1);
		fclose(threshold2);

		return 0;

	}

