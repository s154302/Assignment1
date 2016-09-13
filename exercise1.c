#include <stdio.h>
#include "sensor.c"

int exercise1(){
	FILE* file = openfile("ECG.txt");
	int value = 1, max_value;
	max_value = getNextData(file);

	while(!feof(file)){
		value = getNextData(file);
		if(value > max_value){
			max_value = value;

		}
	}
	printf("%d", max_value);


	return 0;
}
