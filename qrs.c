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
	peaks[100]=count;
	return peaks;

}

void peakDetection(QRS_params *params, int data) {
	static int count = 0, previous = 0;

	int* peaks = findingPeaks(data);
	count++;
	if(previous != peaks[100]){
		count = 0;
	}
	previous = peaks[100];


	printf("%d and %d\n", peaks[peaks[100]], count);


}





