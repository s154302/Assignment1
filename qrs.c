#include "qrs.h"

#include <stdio.h>
#include <stdlib.h>

void arrayInsert(int* array, int size, int value) {
	// Inserts an object into a circular array.
	array[size] = (array[size] + 1) % size;
	if (array[size] < 0) {
		array[size] += size;
	}
	array[array[size]] = value;
}

int peakDetermination(int data, QRS_params *params) {
	// Determinating if there is a peak
	int* peak_determination = params->peak_determination;

	for (int i = 4; i > 0; i--) {
		peak_determination[i] = peak_determination[i - 1];
	}
	peak_determination[0] = data;

	if (peak_determination[0] < peak_determination[1]
			&& peak_determination[1] <= peak_determination[2]
			&& peak_determination[2] >= peak_determination[3]
			&& peak_determination[3] > peak_determination[4]) {

		return peak_determination[2];

	}

	return 0;
}

void findingPeaks(int peak, QRS_params *params) {
	// Saving peaks in an array
	int* array = params->peaks;
	if (peak != 0) {
		arrayInsert(array, 100, peak);
	}
}

void findingTime(int peak, QRS_params *params) {
	// Saving the peaks time in an array
	params->dataTime++;

	if (peak != 0) {
		arrayInsert(params->peaks_time, 100, params->dataTime);

	}
}

int calculateAverage(int* array) {
	// Calculates the average of the array
	// Used for the RR arrays
	int i;
	int average = 0;
	for (i = 0; i < 8; i++) {

		if (array[i] == 0) {
			break;
		}
		average += array[i];

	}

	if (i != 0) {
		return (average / i);
	}
	return 0;
}

void recalculateThresholds(QRS_params *params) {
// Recalculates the thresholds
	params->THRESHOLD1 = params->NPKF + 0.25 * (params->SPKF - params->NPKF);
	params->THRESHOLD2 = 0.5 * params->THRESHOLD1;
}

int peakDetection(QRS_params *params, int data) {

	// Creating/updating the peak arrays
	int peak = peakDetermination(data, params);

	findingPeaks(peak, params);
	int* peaks = params->peaks;

	findingTime(peak, params);
	int* peaks_time = params->peaks_time;

	peak = peaks[peaks[100]];

	int exit = 0;

	// Makes sure it looks at a different peak each time
	if (params->previousPeak != peaks[100]) {

		// If peak is above threshold1 calculate the RR
		if (peak > params->THRESHOLD1) {

			params->RR = peaks_time[peaks_time[100]] - params->RpeakTime;

			// If the RR value is between the high and low RR values then
			if (params->RR_low < params->RR && params->RR < params->RR_high) {

				// Reset warning count
				// Setting exit int
				params->SB_warning = 0;
				exit = 1;

				// Store the peak as an RPeak
				params->Rpeak = peak;
				params->RpeakTime = peaks_time[peaks_time[100]];

				// Recalculate the SPKF
				params->SPKF = 0.125 * peak + 0.875 * params->SPKF;

				// Store the RR value in the recent RR array and the OK RR array
				arrayInsert(params->RR_array, 8, params->RR);
				arrayInsert(params->RR_OK_array, 8, params->RR);

				// Recalculate the averages and the high, low, and miss values
				params->RR_average1 = calculateAverage(params->RR_array);
				params->RR_average2 = calculateAverage(params->RR_OK_array);

				params->RR_low = 0.92 * params->RR_average2;
				params->RR_high = 1.16 * params->RR_average2;
				params->RR_miss = 1.66 * params->RR_average2;


				// Recalculate the thresholds
				recalculateThresholds(params);

			// Else if RR is above RR_miss:
			} else if (params->RR > params->RR_miss) {

				// Counter for peak searchback
				int i = peaks[100] - 1;

				// Loop searching to previous peaks
				while (i != peaks[100]) {

					// If found a peak is above threshold2
					if (peaks[i] > params->THRESHOLD2) {

						// Recalculating RR and saving it
						arrayInsert(params->RR_array, 8, peaks_time[i] - params->RpeakTime);

						// Saving new peak values
						params->Rpeak = peaks[i];
						params->RpeakTime = peaks_time[i];

						params->RR = peaks_time[peaks_time[100]] - params->RpeakTime;

						// Recalculate the averages and the high, low, and miss values
						params->SPKF = 0.125 * peak + 0.875 * params->SPKF;
						params->RR_average1 = calculateAverage(params->RR_array);

						params->RR_low = 0.92 * params->RR_average1;
						params->RR_high = 1.16 * params->RR_average1;
						params->RR_miss = 1.66 * params->RR_average1;

						// Recalculate the thresholds
						recalculateThresholds(params);

						// Setting exit value and breaking array
						exit = 2;
						break;
					}

					// Updating counter
					i = (i - 1) % 100;
					if (i < 0) {
						i += 100;
					}

				}

				// Searching for warnings
				params->SB_warning++;
				if (params->SB_warning > 4) {
					printf("Warning at %d!\n\n", params->seconds);
					params->SB_warning = 0;
				}

			} else {

				// searching for warnings
				params->SB_warning++;
				if (params->SB_warning > 4) {
					printf("Warning at %d!\n\n", params->seconds);
					params->SB_warning = 0;
				}
			}

		} else {
			// Recalculate the NPKF, thresholds and exit
			params->NPKF = 0.125 * peak + 0.875 * params->NPKF;
			recalculateThresholds(params);
			exit = 0;
		}

	}

	// Saving peak number
	params->previousPeak = peaks[100];

	return exit;
}

