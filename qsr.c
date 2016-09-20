#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

void findingPeaks(int data) {
	static int peak_determination[3] = { 0 }, count = -1, peaks[100] = { 0 };

	for (int i = 2; i > 0; i--) {
		peak_determination[i] = peak_determination[i - 1];
	}
	peak_determination[0] = data;

	// Below needs revision.

	if (peak_determination[1] > peak_determination[0]
			&& peak_determination[1] > peak_determination[2]) {
		count = (count + 1) % 100;
		if (count < 0)
			count += 100;
		peaks[count] = peak_determination[1];

	//	printf("%d and %d\n", peaks[count], data);
		/*
		 for(int i = 99; i > 1; i--) {
		 peaks[i] = peaks[i - 1];
		 }
		 peaks[0] = peak_determination[1];
		 */
	}

void peakDetection(QRS_params *params, int data) {
	findingPeaks(data);
	printf("%d and %d\n", peaks[count], data);

}


}


