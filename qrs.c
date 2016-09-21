#include "qrs.h"

#include <stdio.h>
#include <stdlib.h>

int* findingPeaks(int data) {
	static int peak_determination[3] = { 0 }, count = -1, peaks[101] = { 0 };

	for (int i = 2; i > 0; i--) {
		peak_determination[i] = peak_determination[i - 1];
	}
	peak_determination[0] = data;

	if (peak_determination[1] > peak_determination[0]
			&& peak_determination[1] > peak_determination[2]) {
		arrayInsert(peaks, 100, peak_determination[1]);


	}
	return peaks;

}

void arrayInsert(int* array, int size, int value) {
	// Inserts an object into a circular array.
	array[size] = (array[size] + 1) % size;
	if (array[size] < 0) {
		array[size] += size;
	}
	array[array[size]] = value;
}

double calculateAverage(int* array) {
	// Calculates the average of the array
	// Used for the RR arrays
	double average = 0;
	for (int i = 0; i < 8; i++) {
		average += array[i];
	}
	return (average / 8);
}

void recalculateThresholds(QRS_params *params) {
	// Recalculates the thresholds
	params->THRESHOLD1 = params->NPKF + 0.25 * (params->SPKF - params->NPKF);
	params->THRESHOLD2 = 0.5 * params->THRESHOLD1;
}

void peakDetection(QRS_params *params, int data) {
	static int RR_counter = 0, previous_peak = 0,
			Rpeaks[31] = { 0 }, RR_array[9] = { 0 }, RR_OK_array[9] = { 0 },
			RR_average1=80, RR_average2=80,
			RR_low = 10, RR_high = 400, RR_miss = 0,
			first_run = 0;
	int RR;
	if(first_run == 0){
		Rpeaks[30] = 29;
		RR_array[8] = 7;
		RR_OK_array[8] =7;
		first_run++;
	}
	int* peaks = findingPeaks(data);
	int peak = peaks[peaks[100]];

	RR_counter++;
	if (previous_peak != peaks[100]) {
		// If peak is above threshold1 calculate the RR
		if (peak > params->THRESHOLD1) {
			RR = RR_counter;
			RR_counter = 0;

			printf("RRLow: %d, RR: %d, RRHigh: %d\n", RR_low,RR,RR_high);
			// If the RR value is between the high and low RR values then:
			if (RR_low < RR && RR < RR_high) {

				// Store the peak as an RPeak
				arrayInsert(Rpeaks, 30, peak);

				// Recalculate the SPKF
				params->SPKF = 0.125 * peak + 0.875 * params->SPKF;

				// Store the RR value in the recent RR array and the OK RR array
				arrayInsert(RR_array, 8, RR);
				arrayInsert(RR_OK_array, 8, RR);
				//printf("RR: %d and RROk: %d", RR_array[RR_array[8]], RR_OK_array[RR_OK_array[8]]);

				// Recalculate the averages and the high, low, and miss values
				RR_average1 = calculateAverage(RR_array);
				RR_average2 = calculateAverage(RR_OK_array);
				RR_low = 0.92 * RR_average2;
				RR_high = 1.16 * RR_average2;
				RR_miss = 1.66 * RR_average2;

				// Recalculate the thresholds
				recalculateThresholds(params);

			// Else if RR is above RR_miss:
			} else if (RR > RR_miss) {
				// Counter for peak searchback
				int i = peaks[100] - 1;
				while (i != peaks[100]) {
					// If the found peak is above threshold2 then values are recalculated and searchback is stopped.
					if (peaks[i] > params->THRESHOLD2) {
						arrayInsert(Rpeaks, 30, peak);
						params->SPKF = 0.125 * peak + 0.875 * params->SPKF;
						RR_average1 = calculateAverage(RR_array);
						RR_low = 0.92 * RR_average1;
						RR_high = 1.16 * RR_average1;
						RR_miss = 1.66 * RR_average1;
						recalculateThresholds(params);
						break;
					}
					i = (i - 1) % 100;
					if (i < 0) {
						i += 100;
					}
				}
			}
		} else {
			params->NPKF = 0.125 * peak + 0.875 * params->NPKF;
			recalculateThresholds(params);
		}
//printf("%d and %d and %d\n", Rpeaks[30], Rpeaks[Rpeaks[30]], RR_OK_array[RR_OK_array[8]]);
	}
//	printf("%d\n", Rpeaks[Rpeak_counter]);
	previous_peak = peaks[100];

}

