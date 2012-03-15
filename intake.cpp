#include "intake.h"

InTake::InTake (Logger *log, CSVReader *c):
LogBase(log),
config(c),
leftLargeDown(1), leftLargeUp(2), rightLargeDown(3), rightLargeUp(4),
leftLittleDown(5), leftLittleUp(6), rightLittleDown(7), rightLittleUp(8)
{
	lowerMotor = new JaguarLog(log, 9);
	upperMotor = new JaguarLog(log, 10);
}


std::string InTake::name () {
	return "intake";
}



void InTake::log(FILE *f) {
	fprintf(f, ",0");
}

