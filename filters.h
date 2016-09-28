#ifndef FILTERS_H
#define FILTERS_H
#include <stdlib.h>
#include <stdio.h>

// Methods for filters.c
int lowPassFilter(int data);
int highPassFilter(int data);
int derivativeFilter(int data);
int squaringFilter(int data);
int movingWindowIntegrationFilter(int data);
int filterData(int data);

#endif // FILTERS_H
