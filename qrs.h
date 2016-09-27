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
	int SB_Rpeak;
	int SB_Rpeak_time;
	int RR;
	int Rpeak_time;
	// Add parameters that are missing
	// or of use to you

} QRS_params;

// Feel free to change and add methods
int peakDetermination(int data);
void arrayInsert(int* array, int size, int value);
int* findingPeaks(int data);
int* findingTime(int data);
int calculateAverage(int* array);
void recalculateThresholds(QRS_params *params);
int peakDetection(QRS_params *params, int data);

#endif // QRS_H
