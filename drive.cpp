#include "drive.h"
#include "<iostream>"

drive::drive(JaguarLog *l, CSVReader *c):
	LogBase(l),
	config(c)
{
	float scale = config->GetValue("scale");
	int max = config->GetValue("max");
	int min = config->GetValue("min");
	left1 = new JaguarLog(1,2);
	left2 = new JaguarLog(1,3);
	right1 = new JaguarLog(1,4);
	right2 = new JagaurLog(1,5);
}

void drive::fix(float &left, float &right){
	float diff = left - right; 
	float speed = (left+right)/2;
	diff*=scale; 
	left=speed+diff;
	right=speed-diff; 
}

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

void drive::reload(){
	scale = config->GetValue("scale");
	max = config->GetValue("max");
	min = config->GetValue("min");
}

void drive::log(FILE *f) {
	fprintf(f, ",%f", scale);
}

drive::~drive(){
	//something about being deleted by logger?
}