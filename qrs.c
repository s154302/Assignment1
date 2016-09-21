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
		count = (count + 1) % 100;
		if (count < 0)
			count += 100;
		peaks[count] = peak_determination[1];

	}
	peaks[100] = count;
	return peaks;

}

void arrayInsert(int* array, int size, int *counter, int value) {
	// Inserts an object into a circular array.
	counter = (counter + 1) % size;
	if (counter < 0) {
		counter += size;
	}
	array[counter] = value;
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
	static int RR_counter = 0, previous = 0, RR_array_counter = -1,
			Rpeak_counter = -1, OK_RR_counter = -1, Rpeaks[30] = { 0 },
			RR_array[8] = { 0 }, RR_OK_array[8], RR_average1, RR_average2,
			RR_low = 10, RR_high = 400, RR_miss = 0;
	int RR;
	int* peaks = findingPeaks(data);
	int peak = peaks[peaks[100]];

	RR_counter++;
	if (previous != peaks[100]) {
		// If peak is above threshold1 calculate the RR
		if (peak > params->THRESHOLD1) {
			RR = RR_counter;

			RR_counter = 0;

			// If the RR value is between the high and low RR values then:
			if (RR_low < RR && RR < RR_high) {
				// Store the peak as an RPeak
				arrayInsert(Rpeaks, 30, &Rpeak_counter, peak);


				// Recalculate the SPKF
				params->SPKF = 0.125 * peak + 0.875 * params->SPKF;

				// Store the RR value in the recent RR array and the OK RR array
				arrayInsert(RR_array, 8, &RR_array_counter, RR);
				arrayInsert(RR_OK_array, 8, &OK_RR_counter, RR);

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
					if (peaks[i] < params->THRESHOLD2) {
						arrayInsert(Rpeaks, 30, &Rpeak_counter, peak);
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
		printf("%d\n", Rpeaks[Rpeak_counter]);
	}
//	printf("%d\n", Rpeaks[Rpeak_counter]);
	previous = peaks[100];

}

