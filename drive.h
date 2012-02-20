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
	int max;
	int min; 
	CSVReader *config;
	JaguarLog *left1, *left2, *right1, *right2;
public:
	drive (CSVReader*, Logger*);
	virtual ~drive(); 
	void fix(float&, float&);
	void lowpass(float&, float&);
	void fullfast(float&, float&);
	void fullslow(float&, float*);
	void reload(); 
	std::string name();
	void log(FILE*);
};


#endif driveh