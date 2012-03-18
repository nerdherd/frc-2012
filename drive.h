#ifndef driveh
#define driveh

#include "WPILib.h"
#include "CSVReader.h"
#include "Logger.h"
#include "Logged.h"
#include <iostream>
using namespace std;

class Drive : public LogBase {
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
	float currentSpeed;
	//float highSpeed, lowSpeed;
	float alpha;
	float kP, kI, kD;
	
	CSVReader *config;
	JaguarLog *left1, *left2, *right1, *right2;
	
	lowPass leftPass, rightPass;
	PIDout *leftOut, *rightOut;
	PIDController *leftPID, *rightPID;
	
	Encoder leftEncoder, rightEncoder;

	//Relay driveShift;
	Solenoid driveShiftUp, driveShiftDown;
	bool shiftMode;
	
	void fix(float &left, float &right);
	//void lowpass(float&, float&);
public:
	Drive (Logger*, CSVReader*);
	virtual ~Drive(); 
	
	void run(float left, float right);
	
	void shift(bool);
	
	void reload(); 
	std::string name();
	void log(FILE*);
};


#endif // driveh
