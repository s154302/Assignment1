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

void peakDetection(QRS_params *params, int data) {
	static int RR_counter = 0, previous = 0, i = -1, Rpeak_counter = -1, Rpeaks[30] = { 0 }, RR_array[8] = { 0 }, RR_OK_array[8];
	int RR, RR_average2, RR_low, RR_high;
	int* peaks = findingPeaks(data);
	int peak = peaks[peaks[100]];
	RR_counter++;
	if (previous != peaks[100]) {
		if (peak > params->THRESHOLD1) {
			RR = RR_counter;
			RR_counter = 0;
			i = (i + 1) % 8;
			if (i < 0) {
				i += 8;
			}
			RR_array[i] = RR;
			for (int j = 0; j < 8; j++) {
				RR_average2 += RR_array[j];
			}
			RR_average2 = RR_average2 / 8;
			RR_low = 0.92 * RR_average2;
			RR_high = 1.16 * RR_average2;
			if (RR_low < RR && RR < RR_high) {
				Rpeak_counter = (Rpeak_counter + 1) / 30;
				if(Rpeak_counter < 0) {
					Rpeak_counter += 30;
				}
				Rpeaks[Rpeak_counter] = peak;
				params->SPKF = 0.125 * peak + 0.875 * params->SPKF;

			}
		} else {
			params->NPKF = 0.125 * peak + 0.875 * params->NPKF;
			params->THRESHOLD1 = params->NPKF
					+ 0.25 * (params->SPKF - params->NPKF);
			params->THRESHOLD2 = 0.5 * params->THRESHOLD1;
		}
	}
	previous = peaks[100];

}

