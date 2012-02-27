#include "WPILib.h"
#include "CSVReader.h"
#include "Logger.h"
#include "9DOF.h"
//#include "Vision/AxisCamera.h"
#include "Logged.h"
#include "tracking.h"
#include "shooter.h"
#include "drive.h"

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
	CameraTracking *camera;
	IMU *imu;
public:
	Robot(void):
		DriverStick(1)
		,ArticStick(2)
	{
		Wait(.5);
		config = new CSVReader("Config.csv");
		log = new Logger("MatchLog",0.1);
		
		camera = new CameraTracking(log, config);
		
		drive = new Drive(log, config);
		shooter = new Shooter(log, config, camera);
		
		//imu = new IMU(log);
		
		log->init();
	}
	

	~Robot () {
		delete log; // everything connected with the logger should be deleted when this deleted
		delete config;
	}
	
	void Init () {
		config->ReloadValues();
		shooter->reload();
		drive->reload();
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
			float left = DriverStick.GetRawAxis(2);
			float right = DriverStick.GetRawAxis(4);
			drive->run(left, right);
			
			
			
			Wait(.01);
		}
	}
};

START_ROBOT_CLASS(Robot);
