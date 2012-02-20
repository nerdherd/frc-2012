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
	class lowPass {
	private:
		float lastValue;
		float *alpha;
	public:
		lowPass(float *c):lastValue(0), alpha(c) {}
		float operator() (float value);
	};
	class PIDout : public PIDOutput {
	private:
		JaguarLog *motor1, *motor2;
	public:
		PIDout (JaguarLog*, JaguarLog*);
		void PIDWrite(float output);
		
	};
	
	
	float scale;
	float highSpeed;
	float lowSpeed; 
	float alpha;
	float kP, kI, kD;
	
	CSVReader *config;
	JaguarLog *left1, *left2, *right1, *right2;
	
	lowPass leftPass, rightPass;
	PIDout *leftOut, *rightOut;
	PIDController *leftPID, *rightPID;
	
	Encoder leftEncoder, rightEncoder;
	
	void fix(float &left, float &right);
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


#endif // driveh
