#include <stdio.h>

// Exercise 1b
int main(){
	static const char filename[] = "ECG.txt";
	FILE *file = fopen(filename, "r");
	int value = 1, max_value;
	fscanf(file, "%d", &max_value);

	while(!feof(file)){
		fscanf(file, "%d", &value);
		if(value > max_value){
			max_value = value;

		}
	}
	printf("%d", max_value);


	return 0;
}

