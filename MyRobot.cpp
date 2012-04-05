#include "WPILib.h"
#include "CSVReader.h"
#include "Logger.h"
#include "9DOF.h"
//#include "Vision/AxisCamera.h"
#include "Logged.h"
#include "tracking.h"
#include "shooter.h"
#include "drive.h"
#include "intake.h"

#include <iostream>
using namespace std;

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class Robot : public SimpleRobot
{
private:
	CSVReader *config;
	Logger *log;
	
	Joystick DriverStick, ArticStick; 
	
	Shooter *shooter;
	Drive *drive;
	InTake *intake;
	CameraTracking *camera;
	//IMU *imu;

	Compressor compressor;
public:
	Robot(void):
		DriverStick(1)
		,ArticStick(2)
		,compressor(14,1)
	{
		
		Wait(.5);
		config = new CSVReader("Config.csv");
		log = new Logger("MatchLog",0.1);
		
		camera = new CameraTracking(log, config);
		
		drive = new Drive(log, config);
		//shooter = NULL;
		//shooter = new Shooter(log, config, camera);
		intake = new InTake(log, config);
		
		//imu = new IMU(log);
		
		log->init();
		compressor.Start();
	}
	

	~Robot () {
		delete log; // everything connected with the logger should be deleted when this deleted
		delete config;
	}
	
	void Init () {
		config->ReloadValues();
		if(shooter) shooter->reload();
		if(drive) drive->reload();
	}
	
	void Disabled (void) {
		//shooter1->Set(0);
		//shooter2->Set(0);
	}
	
	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous(void)
	{
		Init();
		GetWatchdog().SetEnabled(false);
		cout << "Autonomous running\n";
		while(IsAutonomous() && IsEnabled()) {
			//shooter->run();
			Wait(.1);
		}
		
		GetWatchdog().SetEnabled(true);
	}

	void OperatorControl(void)
	{
		Init();
		GetWatchdog().SetEnabled(true);
		cout << "Operator Control running\n";
		while(IsOperatorControl()) {
			GetWatchdog().Feed();
			//WaitForData();
			while(!IsNewDataAvailable()) Wait(.001); // wait until the driver station sends data to the robot
			if(DriverStick.GetRawButton(5)) {
				drive->shift(true);
			}else if(DriverStick.GetRawButton(7)) {
				drive->shift(false);
			}
			float left = DriverStick.GetRawAxis(2);
			float right = DriverStick.GetRawAxis(4);
			drive->run(left, right);
			
			intake->run(-1*ArticStick.GetRawAxis(2), ArticStick.GetRawAxis(4), false, false, false);
			
			
			Wait(.01);
		}
	}
	
};

START_ROBOT_CLASS(Robot);
