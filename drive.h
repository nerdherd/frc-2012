#ifndef driveh
#define driveh

#include "WPILib.h"
#include "CSVReader.h"
#include "Logger.h"
#include "Logged.h"
#include <iostream>
using namespace std;

class drive : public LogBase {
private:
	float scale; 
	CSVReader *c;
	JaguarLog *l; 
	
public:
	drive(Logger* l, CSVReader* c);
	~drive(); 
	void fix(float, float);
	void reload();
};







#endif driveh