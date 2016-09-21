#ifndef QRS_H
#define QRS_H

// Header file for QRS functionality 
// it is recommended to use the structure "QRS_parameters"
// to organize all variables and parameters

typedef struct QRS_params { // Structure for QRS parameters
	double SPKF;
	double NPKF;
	double THRESHOLD1;
	double THRESHOLD2;
	// Add parameters that are missing
	// or of use to you

} QRS_params;

// Feel free to change and add methods
int* findingPeaks(int data);
void arrayInsert(int* array, int size, int value);
double calculateAverage(int* array);
void recalculateThresholds(QRS_params *params);
int peakDetection(QRS_params *params, int data);

#endif // QRS_H
