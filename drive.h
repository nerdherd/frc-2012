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
	float highSpeed;
	float lowSpeed; 
	CSVReader *config;
	JaguarLog *left1, *left2, *right1, *right2;
	void fix(float&, float&);
	//void lowpass(float&, float&);
public:
	drive (CSVReader*, Logger*);
	virtual ~drive(); 
	
	
	//void fullfast(float&, float&);
	//void fullslow(float&, float*);
	void reload(); 
	std::string name();
	void log(FILE*);
};


#endif driveh