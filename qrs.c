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

int peakDetermination(int data) {
	// Determinating if there is a peak
	static int peak_determination[5] = { 0 };

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

int* findingPeaks(int peak) {
	// Saving peaks in an array
	static int peaks[101] = { 0 };

	if (peak != 0) {
		arrayInsert(peaks, 100, peak);
	}

	return peaks;
}

int* findingTime(int peak) {
	// Saving the peaks time in an array
	static int peaks_time[101] = { 0 }, time = 0;
	time++;

	if (peak != 0) {
		arrayInsert(peaks_time, 100, time);

	}
	return peaks_time;
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
	// Analysing the peaks
	static int previous_peak = 0, RR_array[9] = { 0 }, RR_OK_array[9] = { 0 },
			RR_average1 = 80, RR_average2 = 80, RR_low = 10, RR_high = 200,
			RR_miss = 300, first_time = 0, SBwarning = 0;
	int exit = 0;

	// Initiating the RR arrays
	if (first_time == 0) {
		RR_array[8] = 7;
		RR_OK_array[8] = 7;
		first_time++;
	}


	// Creating/updating the peak arrays
	int peak = peakDetermination(data);
	int* peaks = findingPeaks(peak);
	int* peaks_time = findingTime(peak);
	peak = peaks[peaks[100]];

	// Makes sure it looks at a different peak each time
	if (previous_peak != peaks[100]) {

		// If peak is above threshold1 calculate the RR
		if (peak > params->THRESHOLD1) {

			params->RR = peaks_time[peaks_time[100]] - params->RpeakTime;

			// If the RR value is between the high and low RR values then
			if (RR_low < params->RR && params->RR < RR_high) {

				// Reset warning count
				// Setting exit int
				SBwarning = 0;
				exit = 1;

				// Store the peak as an RPeak
				params->Rpeak = peak;
				params->RpeakTime = peaks_time[peaks_time[100]];

				// Recalculate the SPKF
				params->SPKF = 0.125 * peak + 0.875 * params->SPKF;

				// Store the RR value in the recent RR array and the OK RR array
				arrayInsert(RR_array, 8, params->RR);
				arrayInsert(RR_OK_array, 8, params->RR);

				// Recalculate the averages and the high, low, and miss values
				RR_average1 = calculateAverage(RR_array);
				RR_average2 = calculateAverage(RR_OK_array);

				RR_low = 0.92 * RR_average2;
				RR_high = 1.16 * RR_average2;
				RR_miss = 1.66 * RR_average2;

				// Recalculate the thresholds
				recalculateThresholds(params);

			// Else if RR is above RR_miss:
			} else if (params->RR > RR_miss) {

				// Counter for peak searchback
				int i = peaks[100] - 1;

				// Loop searching to previous peaks
				while (i != peaks[100]) {

					// If found a peak is above threshold2
					if (peaks[i] > params->THRESHOLD2) {

						// Recalculating RR and saving it
						arrayInsert(RR_array, 8, peaks_time[i] - params->RpeakTime);

						// Saving new peak values
						params->Rpeak = peaks[i];
						params->RpeakTime = peaks_time[i];
						params->RR = peaks_time[peaks_time[100]] - params->RpeakTime;

						// Recalculate the averages and the high, low, and miss values
						params->SPKF = 0.125 * peak + 0.875 * params->SPKF;
						RR_average1 = calculateAverage(RR_array);

						RR_low = 0.92 * RR_average1;
						RR_high = 1.16 * RR_average1;
						RR_miss = 1.66 * RR_average1;

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
				SBwarning++;
				if (SBwarning > 4) {
					printf("Warning at %d!\n\n", params->seconds);
					SBwarning = 0;
				}

			} else {

				// searching for warnings
				SBwarning++;
				if (SBwarning > 4) {
					printf("Warning at %d!\n\n", params->seconds);
					SBwarning = 0;
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
	previous_peak = peaks[100];

	return exit;
}

