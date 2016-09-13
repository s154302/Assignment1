#include "filters.h"
#include <stdlib.h>
#include <stdio.h>

// Feel free to change return statement and arguments
int lowPassFilter(int data) {
    static int count, previous_1, previous_2, data_6, data_12, current;
    
    current = 2 * previous_1 - previous_2 + (data - 2 * data_6 + data_12) / 32;
    previous_2 = previous_1;
    previous_1 = current;

    return current;
}
