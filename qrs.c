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
	static int peak_determination[5] = { 0 };

	for (int i = 4; i > 0; i--) {
		peak_determination[i] = peak_determination[i - 1];
	}
	peak_determination[0] = data;

	if(peak_determination[0] < peak_determination[1] && peak_determination[1] <= peak_determination[2] &&
			peak_determination[2] >= peak_determination[3] && peak_determination[3] > peak_determination[4] ){

		return peak_determination[2];

	}

	return 0;
}

int* findingPeaks(int peak) {
	static int peaks[101] = { 0 };

	if (peak != 0) {
		arrayInsert(peaks, 100, peak);
	}

	return peaks;
}

int* findingTime(int peak) {
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

	i *= 100;
	average *= 100;
	return (average / i);
}

void recalculateThresholds(QRS_params *params) {
// Recalculates the thresholds
	params->THRESHOLD1 = params->NPKF + 0.25 * (params->SPKF - params->NPKF);
	params->THRESHOLD2 = 0.5 * params->THRESHOLD1;
}

int peakDetection(QRS_params *params, int data) {
	static int RR_counter = -3, previous_peak = 0, RR_array[9] = { 0 },
			RR_OK_array[9] = { 0 }, RR_average1 = 80, RR_average2 = 80, RR_low =
					140, RR_high = 200, RR_miss = 300, RR = 0, first_time = 0, SBwarning = 0;
	int exit = 0;

	if (first_time == 0) {
		RR_array[8] = 7;
		RR_OK_array[8] = 7;
		first_time++;
	}

	int peak_data = peakDetermination(data);
	int* peaks = findingPeaks(peak_data);
	int* peaks_time = findingTime(peak_data);
	int peak = peaks[peaks[100]];

	RR_counter++;

	// Analysing new peak
	if (previous_peak != peaks[100]) {
		// If peak is above threshold1 calculate the RR
		if (peak > params->THRESHOLD1) {
			RR = RR_counter;
			params->RR = RR_counter;

			// If the RR value is between the high and low RR values then:
			if (RR_low < RR && RR < RR_high) {

				// Reset RR counter
				RR_counter = 0;
				SBwarning = 0;
				exit = 1;

				// Store the peak as an RPeak
				params->Rpeak = peak;
				params->Rpeak_time = peaks_time[peaks_time[100]];

				// Recalculate the SPKF
				params->SPKF = 0.125 * peak + 0.875 * params->SPKF;

				// Store the RR value in the recent RR array and the OK RR array
				arrayInsert(RR_array, 8, RR);
				arrayInsert(RR_OK_array, 8, RR);

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

						if (peaks_time[i] - params->Rpeak_time > 1000) {
							arrayInsert(RR_array, 8, RR_average1);
						} else {
							arrayInsert(RR_array, 8,
									peaks_time[i] - params->Rpeak_time);
						}

						RR_counter = peaks_time[peaks_time[100]]
								- peaks_time[i];

						params->SB_Rpeak = peaks[i];
						params->SB_Rpeak_time = peaks_time[i];

						params->SPKF = 0.125 * peak + 0.875 * params->SPKF;
						RR_average1 = calculateAverage(RR_array);
						RR_low = 0.92 * RR_average1;
						RR_high = 1.16 * RR_average1;
						RR_miss = 1.66 * RR_average1;
						recalculateThresholds(params);
						params->RR = RR_array[RR_array[8]];

						exit = 1;
						break;
					}
					i = (i - 1) % 100;
					if (i < 0) {
						i += 100;
					}

				}
			}
			if (RR_low > RR || RR > RR_high) {
				SBwarning++;
				if (SBwarning > 4) {
					printf("Warning at %d\n", params->seconds);

				}
			}
		} else {
			params->NPKF = 0.125 * peak + 0.875 * params->NPKF;
			recalculateThresholds(params);
			exit = 0;
		}
	}
	previous_peak = peaks[100];
	return exit;
}

