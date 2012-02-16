#ifndef _shooter_h
#define _shooter_h

#include "WPILib.h"

#include "Logged.h"
#include "CSVReader.h"
#include "tracking.h"

class Shooter: public LogBase, public PIDOutput {
private:
	JaguarLog *motor1, *motor2;
	CSVReader *config;
	Encoder encoder;
	PIDController *pid;
public:
	Shooter (CSVReader*, Logger*);
	void reload();
	void run();
	
	virtual void PIDWrite(float output);
	virtual ~Shooter();
	
	std::string name();
	void log(FILE*);
};

#endif // _shooter_h
