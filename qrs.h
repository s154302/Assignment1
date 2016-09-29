#ifndef QRS_H
#define QRS_H

// Header file for QRS functionality 
// it is recommended to use the structure "QRS_parameters"
// to organize all variables and parameters

typedef struct QRS_params { // Structure for QRS parameters
	// Estimated Rpeak value
	int SPKF;

	// Estimated noise peak value
	int NPKF;

	// Thresholds for Rpeaks
	int THRESHOLD1;
	int THRESHOLD2;

	// Rpeak values
	int Rpeak;

	// Distance between two most recent Rpeaks
	int RR;

	// Time elapsed in seconds
	int seconds;

	// Counts time in number of data points
	int dataTime;

	// Datapoint of most recen Rpeak
	int RpeakTime;

	// Variable to make sure detection only runs on new peaks
	int previousPeak;

	// Array of most recent RR values
	int* RR_array;

	// Array of most recent RR values of Rpeaks above thresh1
	int* RR_OK_array;

	// Array of peaks
	int* peaks;

	// Array of peak times
	int* peaks_time;

	// Array to determine whether or not a data point is a peak
	int* peak_determination;

	// Average of RR_array
	int RR_average1;

	// Average of RR_OK_array
	int RR_average2;

	// Lower limit of RR_OK
	int RR_low;

	// Upper limit of RR_OK
	int RR_high;

	// RR value which should initiate a searchback
	int RR_miss;

	// Variable to initialise various variables on the first run
	int first_time;

	// Counter used for warnings
	int SB_warning;

} QRS_params;

// Methods for qrs.c
int peakDetermination(int data, QRS_params *params);
void arrayInsert(int* array, int size, int value);
void findingPeaks(int data, QRS_params *params);
void findingTime(int data, QRS_params *params);
int calculateAverage(int* array);
void recalculateThresholds(QRS_params *params);
int peakDetection(QRS_params *params, int data);

#endif // QRS_H
