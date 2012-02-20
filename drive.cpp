#include "drive.h"

drive::drive(CSVReader *c, Logger *l):
	LogBase(l),
	config(c)
{
	scale = config->GetValue("turnScale");
	highSpeed = config->GetValue("driveHigh");
	lowSpeed = config->GetValue("driveLow");
	left1 = new JaguarLog(l,2);
	left2 = new JaguarLog(l,3);
	right1 = new JaguarLog(l,4);
	right2 = new JaguarLog(l,5);
}

void drive::fix(float &left, float &right){
	float diff = left - right; 
	float speed = (left+right)/2;
	diff*=scale; 
	left=speed+diff;
	right=speed-diff; 
}
/*
void drive::lowpass(float &left, float &right){
	
}

void drive::fullfast(float &left, float &right){
	left = max;
	right = max; 
}

void drive::fullslow(float &left, float &right){
	left = min;
	right = min;
}
*/

void drive::reload(){
	scale = config->GetValue("turnScale");
	highSpeed = config->GetValue("driveHigh");
	lowSpeed = config->GetValue("driveLow");
	//max = config->GetValue("max");
	//min = config->GetValue("min");
}

void drive::log(FILE *f) {
	fprintf(f, ",%f", scale);
}

drive::~drive(){
	//something about being deleted by logger?
}