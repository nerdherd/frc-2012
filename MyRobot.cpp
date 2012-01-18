 #include "WPILib.h"
#include "CSVReader.h"
#include "Logger.h"
#include "9DOF.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class Robot : public SimpleRobot
{
	//RobotDrive myRobot; // robot drive system
	Joystick stick; // only joystick
	Jaguar left1;

	CSVReader *config;
	Logger *log;
	IMU *imu;
public:
	Robot(void):
		//myRobot(1, 2),	// these must be initialized in the same order
		stick(1)		// as they are declared above.
		,left1(1)
	{
		//myRobot.SetExpiration(0.1);
		config = new CSVReader("Config.csv");
		log = new Logger("MatchLog.csv",5);
		imu = new IMU;
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous(void)
	{
		GetWatchdog().SetEnabled(false);
		//myRobot.SetSafetyEnabled(false);
		//myRobot.Drive(0.5, 0.0); 	// drive forwards half speed
		//Wait(2.0); 				//    for 2 seconds
		//myRobot.Drive(0.0, 0.0); 	// stop robot
		while(true) {
			imu->update();
			Wait(.1);
		}
		
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl(void)
	{
		/*
		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
			myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)
			Wait(0.005);				// wait for a motor update time
		}
		*/
		left1.Set(.2);
	}
};

START_ROBOT_CLASS(Robot);

