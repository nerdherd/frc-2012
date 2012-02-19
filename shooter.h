#ifndef _shooter_h
#define _shooter_h

#include "WPILib.h"

#include "Logged.h"
#include "CSVReader.h"
#include "tracking.h"

class Shooter: public LogBase, public PIDOutput {
private:
	JaguarLog *motor1, *motor2, *turret;
	CSVReader *config;
	PIDController *pid;
	CameraTracking *camera;
	
	Encoder encoder;
	AnalogChannel distance;
	Task TaskShoot;
	DigitalInput LimitTurret;
	// The location will be mod 1, 1 will be a complete rotation
	float TurretLocation;
	bool TurretSweepDir;
	bool inited;
	
	float previousDistance;
	float computeDistance ();
	
	float computeSpeed(float);
	void computeTurn();
	static void s_TaskShoot(Shooter*);
public:
	Shooter (CSVReader*, Logger*, CameraTracking*);
	void reload();
	void run();
	
	virtual void PIDWrite(float output);
	virtual ~Shooter();
	
	std::string name();
	void log(FILE*);
};

#endif // _shooter_h
