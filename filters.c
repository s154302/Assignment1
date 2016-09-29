#include "filters.h"
#include <stdlib.h>
#include <stdio.h>

// The low pass filter
int lowPassFilter(int data) {
	static int previous_1 = 0, previous_2 = 0, current = 0, data_array[12] =
			{ 0 };

	current = 2 * previous_1 - previous_2
			+ (data - 2 * data_array[5] + data_array[11]) / 32;
	previous_2 = previous_1;
	previous_1 = current;

	for (int i = 11; i > 0; i--) {
		data_array[i] = data_array[i - 1];
	}
	data_array[0] = data;

	return current;
}

// The high pass filter
int highPassFilter(int data) {
	static int current = 0, previous_1 = 0, data_array[32] = { 0 };

	current = previous_1 - data / 32 + data_array[15] - data_array[16]
			+ data_array[31] / 32;
	previous_1 = current;
	for (int i = 31; i > 0; i--) {
		data_array[i] = data_array[i - 1];
	}
	data_array[0] = data;
	return current;
}

// The derivative filter
int derivativeFilter(int data) {
	static int current = 0, data_array[4] = { 0 };

	current = (2 * data + data_array[0] - data_array[2] - 2 * data_array[3])
			/ 8;

	for (int i = 3; i > 0; i--) {
		data_array[i] = data_array[i - 1];
	}
	data_array[0] = data;
	return current;
}

// The squaring filter
int squaringFilter(int data) {
	return data * data;
}

// The moving window integration filter
int movingWindowIntegrationFilter(int data) {
	static int current = 0, data_array[30] = { 0 };
	int sum = 0;

	data_array[0] = data;
	for (int i = 0; i < 30; i++) {
		sum += data_array[i];
	}

	current = sum / 30;

	for (int i = 29; i > 0; i--) {
		data_array[i] = data_array[i - 1];
	}
	return current;
}

// Filtering the data
int filterData(int data) {

	return movingWindowIntegrationFilter(
			squaringFilter(
					derivativeFilter(highPassFilter(lowPassFilter(data)))));
}
