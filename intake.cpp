#include "intake.h"

InTake::InTake (Logger *log, CSVReader *c):
LogBase(log),
config(c),
leftLargeDown(1), leftLargeUp(2), rightLargeDown(3), rightLargeUp(4),
leftLittleDown(5), leftLittleUp(6), rightLittleDown(7), rightLittleUp(8)
{
	leftDown = rightDown = false;
	lowerMotor = new JaguarLog(log, 9);
	//upperMotor = new JaguarLog(log, 10);
	
}

void InTake::run(float lower, bool feedF, bool feedB, bool leftPop, bool rightPop, bool littlePop) {
	lowerMotor->Set(lower);
	//upperMotor->Set(feedF ? 1.0 : (feedB ? -1.0 : 0));
	if(leftDown || rightDown) {
		if(rightPop) {
			if(leftDown && !leftPop) {
				leftDown = false;
				rightDown = true;
			}
		}else if(leftPop) {
			if(rightDown && !rightPop) {
				rightDown = false;
				leftDown = true;
			}
		}else{
			leftDown = false;
			rightDown = false;
		}
	}else{
		if(leftPop) leftDown = true;
		else if(rightPop) rightDown = true;
	}
	
	// set the values to the solonoids
	if(leftPop) {
		leftLargeDown.Set(true);
		leftLargeUp.Set(false);
		leftLittleDown.Set(littlePop);
		leftLittleUp.Set(!littlePop);
	}else{
		leftLargeDown.Set(false);
		leftLargeUp.Set(true);
		leftLittleDown.Set(false);
		leftLittleUp.Set(true);
	}
	if(rightPop) {
		rightLargeDown.Set(true);
		rightLargeUp.Set(false);
		rightLittleDown.Set(littlePop);
		rightLittleUp.Set(!littlePop);
	}else{
		rightLargeDown.Set(false);
		rightLargeUp.Set(true);
		rightLittleDown.Set(false);
		rightLittleUp.Set(true);
	}
}

std::string InTake::name () {
	return "intake";
}



void InTake::log(FILE *f) {
	fprintf(f, ",%f", upperMotor->Get());
}

