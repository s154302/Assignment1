#ifndef QRS_H
#define QRS_H

// Header file for QRS functionality 
// it is recommended to use the structure "QRS_parameters"
// to organize all variables and parameters

typedef struct QRS_params { // Structure for QRS parameters
	int SPKF;
	int NPKF;
	int THRESHOLD1;
	int THRESHOLD2;
	int Rpeak;
	int RR;
	int seconds;
	int RpeakTime;


} QRS_params;

// Methods for qrs.c
int peakDetermination(int data);
void arrayInsert(int* array, int size, int value);
int* findingPeaks(int data);
int* findingTime(int data);
int calculateAverage(int* array);
void recalculateThresholds(QRS_params *params);
int peakDetection(QRS_params *params, int data);

#endif // QRS_H
