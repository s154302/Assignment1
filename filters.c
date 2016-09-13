#include "filters.h"

// Feel free to change return statement and arguments
int lowPassFilter(int data)
{
	static const int count, previous1, previous2, data6, data12, current;
	if(count==0){
		current = (1/32)*data;
	} else if (count == 1){
		previous1 = current;
	} else if (count >=2 && count < 6){
		previous2 = previous1;
		previous1 = current;
		current = 2*previous1 - previous2 + (1/32)*(data-2*data6+data12);
	} else if(count >= 6 && count < 12){

	} else {
		current = 2*previous1-previous2 + (1/32)*(data-2*data6+data12);
	}
	// Implement additional filters
	current = 2*previous1 - previous2 + (1/32)*(data-2*data6+data12);
	if(count <= 12){
		count++;
	}
	return current;
}
