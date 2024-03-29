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

extern double getoShooterSpeed;

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
		
		camera = NULL;
	//	camera = new CameraTracking(log, config);
		
		drive = new Drive(log, config);
		shooter = NULL;
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
		//if(camera) camera->reload();
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
		//while(IsAutonomous() && IsEnabled()) {
			//shooter->run();
		//	Wait(.1);
		//}
		/*getoShooterSpeed=3500;
		Wait(config->GetValue("autoWaitTime"));
		intake->run(1,false, true, false, false, false);
		Wait(.6);
		intake->run(0,false, false, false, false, false);
		getoShooterSpeed=3600;
		Wait(3.0);
		intake->run(1, false, true, false, false, false);
		Wait(5.0);*/
		
		GetWatchdog().SetEnabled(true);
	}

	void OperatorControl(void)
	{
		Init();
		GetWatchdog().SetEnabled(true);
		cout << "Operator Control running\n";
		while(IsOperatorControl() && IsEnabled()) {
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
			
			intake->run(-1*ArticStick.GetRawAxis(2), ArticStick.GetRawButton(5), ArticStick.GetRawButton(7), false, false, false);
			if(shooter) shooter->runTurret(ArticStick.GetRawAxis(3));
			
			//getoShooterSpeed += ArticStick.GetRawAxis(4) * .5;
			if(ArticStick.GetRawButton(6))
				getoShooterSpeed += 25;
			if(ArticStick.GetRawButton(8))
				getoShooterSpeed -= 25;
			if(ArticStick.GetRawButton(10))
				getoShooterSpeed = 0;
			if(ArticStick.GetRawButton(9))
				getoShooterSpeed = config->GetValue("shooter");
			if(ArticStick.GetRawButton(1))
				getoShooterSpeed = 2000;
			if(ArticStick.GetRawButton(2))
				getoShooterSpeed = 3000;
			if(ArticStick.GetRawButton(3))
				getoShooterSpeed = 4000;
			if(ArticStick.GetRawButton(4))					
				getoShooterSpeed = 3500;//5000;
			
			Wait(.01);
		}
	}
	
};

START_ROBOT_CLASS(Robot);
