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
public:
	InTake(Logger*, CSVReader*);
	
	void run (float lower, float upper, bool leftPop, bool rightPop);
	
	std::string name ();
	void log (FILE*);
};

#endif // _nerdherd_intake

