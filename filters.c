#include "filters.h"
#include <stdlib.h>
#include <stdio.h>

// Feel free to change return statement and arguments
int lowPassFilter(int data) {
    static int previous_1, previous_2, current, first;
    static int data_array[12];

    current = 2 * previous_1 - previous_2 + (data - 2 * data_array[5] + data_array[11]) / 32;
    previous_2 = previous_1;
    previous_1 = current;

    for(int i = 11; i > 0;i--){
    	data_array[i] = data_array[i-1];
    }
    data_array[0] = data; 
    
    return current;
}

int highPassFilter(int data){
	static int current, previous_1;
	static int data_array[32];
	current = previous_1 - data / 32 + data_array[15] - data_array[16] + data_array[31] / 32;
	previous_1 = current;
	for(int i = 31; i>0; i--){
		data_array[i] = data_array[i-1];
	}
	data_array[0]=data;
	return current;
}

int derivativeFilter(int data){
	static int current;
	static int data_array[4];
		
	current = (2*data + data_array[0] - data_array[2] -2 * data_array[3]) / 8;
		
		for(int i = 3; i>0; i--){
			data_array[i] = data_array[i-1];
		}
		data_array[0]=data;
		return current;
}

int squaringFilter(int data) {
    return data * data;
}

int movingWindowIntegrationFilter(int data) {
    static int current = 0, data_array[29];
    
    for(int i = 0; i < 29; i++) {
        printf(" %d", data_array[i]);
    }
    printf("\n");
    
    for(int i = 0; i < 29; i++) {
        current = current + data_array[i];
    }
   // printf("%d\n", current + data);
    
    current = (current + data) / 30;
    
    for(int i = 28; i > 0; i--) {
        data_array[i] = data_array[i-1];
    }
    data_array[0] = data;
    
    return current;
}
