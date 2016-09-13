#include "filters.h"
#include <stdlib.h>
#include <stdio.h>

// Feel free to change return statement and arguments
int lowPassFilter(int data) {
    static int count, previous_1, previous_2, data_6, data_12, current;
    int data_array[12];
    
<<<<<<< HEAD
    current = 2 * previous_1 - previous_2 + (data - 2 * data_6 + data_12) / 32;
    previous_2 = previous_1;
    previous_1 = current;
=======
    current = 2 * previous_1 - previous_2 + (data-2*data_array[5]+data_array[11]) / 32;
    previous_2 = previous_1;
    previous_1 = current;

    for(int i = 11; i>=0;i--){
    	data_array[i] = data_array[i-1];
    }
	data_array[0] = data;
>>>>>>> origin/master

    return current;
}
