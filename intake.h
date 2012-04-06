#ifndef _nerdherd_intake
#define _nerdherd_intake

#include "CSVReader.h"
#include "Logged.h"
#include "Logger.h"

class InTake : public LogBase {
private:
	CSVReader *config;
	JaguarLog *lowerMotor, *upperMotor;
	Solenoid leftLargeDown, leftLargeUp, rightLargeDown, rightLargeUp;
	Solenoid leftLittleDown, leftLittleUp, rightLittleDown, rightLittleUp;
	bool leftDown, rightDown;
public:
	InTake(Logger*, CSVReader*);
	
	void run (float lower, bool feedF, bool feedB, bool leftPop, bool rightPop, bool littlePop);
	
	std::string name ();
	void log (FILE*);
};

#endif // _nerdherd_intake

