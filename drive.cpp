#include "drive.h"
#include "<iostream>"

drive::drive(JaguarLog *l, CSVReader *c){
	
}

void drive::fix(float &left, float &right){
	float diff = left - right; 
	float speed = (left+right)/2;
	diff*=scale; 
	left=speed+diff;
	right=speed-diff; 
}

int main(){
	
}