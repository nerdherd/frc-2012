#include "WPILib.h"
#include "CSVReader.h"
#include "Logger.h"
#include "9DOF.h"
//#include "Vision/AxisCamera.h"
#include "Logged.h"
#include "tracking.h"

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
	//RobotDrive myRobot; // robot drive system
	Joystick stick; // only joystick
	//Jaguar left1;//, left2, right1, right2;
	//CANJaguar left1, left2, right1, right2;
	JaguarLog *shooter1, *shooter2;
	JaguarLog *left1, *left2, *right1, *right2;
	Encoder shooterEncoder;
	Relay rr;
	
	AnalogChannel distance;
	
	CSVReader *config;
	Logger *log;
	CameraTracking *camera;
	IMU *imu;
public:
	Robot(void):
		//myRobot(1, 2),	// these must be initialized in the same order
		stick(1)		// as they are declared above.
		/*,left1(2)
		,left2(4)
		,right1(5)
		,right2(3)*/
		,distance(2)
		//,shooter(8)
		/*,left2(2)
		,right1(3)
		,right2(4)*/
		,shooterEncoder(1,2)
		,rr(2)
	{
		//myRobot.SetExpiration(0.1);
		Wait(.5);
		config = new CSVReader("Config.csv");
		log = new Logger("MatchLog.csv",0.1);
		//shooter = new JaguarLog(log, 7, CANJaguar::kSpeed);
		/*left1 = new JaguarLog(log, 2);
		left2 = new JaguarLog(log, 4);
		right1 = new JaguarLog(log, 5);
		right2 = new JaguarLog(log, 3);
		*/
		//camera = new CameraTracking(log, config);
		
		//imu = new IMU(log);
		
		log->init();
	}

	~Robot () {
		//delete shooter;
		delete log; // everything connected with the logger should be deleted when this deleted
		delete config;
	}
	
	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous(void)
	{
		
		GetWatchdog().SetEnabled(false);
		cout << "Autonomous running\n";
		//myRobot.SetSafetyEnabled(false);
		//myRobot.Drive(0.5, 0.0); 	// drive forwards half speed
		//Wait(2.0); 				//    for 2 seconds
		//myRobot.Drive(0.0, 0.0); 	// stop robot
		//Jaguar left1(1);
		/*AxisCamera &camera = AxisCamera::GetInstance("10.06.87.11");
		HSLImage *image = new HSLImage();
		cout << camera.GetImage(image) << endl;*/
		/*while(IsAutonomous()) {
			/*left1.Set(.2);
			left2.Set(.2);
			right1.Set(.2);
			right2.Set(.2);
			//cout << distance.GetVoltage()/.0098 << "\n";
			//	<< camera.GetImage(image) << endl;
			printf("auto running\n");
			Wait(.1);
			//imu->update();
			//cout << left1.IsAlive() << " " << left1.IsSafetyEnabled() << endl;
		}*/
		while(IsAutonomous()) {
			rr.Set(Relay::kOn);
			Wait(.1);
			printf("encoder %i\n", shooterEncoder.Get());
		}
		
		GetWatchdog().SetEnabled(true);
		/*
		GetWatchdog().SetEnabled(false);
		config->ReloadValues();
		cout << config->GetValue("shooter") << endl;
		shooter->EnableControl(0);
		shooter->ConfigEncoderCodesPerRev(250);
		shooter->SetPID(config->GetValue("shooterP"), config->GetValue("shooterI"), config->GetValue("shooterD"));
		Wait(.5);
		double speed = config->GetValue("shooter");
		
		while(IsAutonomous() && IsEnabled()) {
			shooter->Set(speed);
			cout << shooter->GetSpeed() << " " << shooter->Get() << endl;
			Wait(.1);
			
		}
		shooter->Set(0);
		GetWatchdog().SetEnabled(true);
		*/
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl(void)
	{
		GetWatchdog().SetEnabled(true);
		/*
		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
			myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)
			Wait(0.005);				// wait for a motor update time
		}
		*/
		cout << "Operator Control running\n";
		while(IsOperatorControl()) {
			GetWatchdog().Feed();
			//left1.SetSpeed(1);
			/*left2.SetSpeed(1);
			right1.SetSpeed(1);
			right2.SetSpeed(1);*/
			//cout << left1.IsAlive() << " " << left1.IsSafetyEnabled() << endl;
			right1->Set(stick.GetRawAxis(2));
			right2->Set(stick.GetRawAxis(2));
			left1->Set(-1*stick.GetRawAxis(4));
			left2->Set(-1*stick.GetRawAxis(4));
			Wait(.02);
		}
	}
};

START_ROBOT_CLASS(Robot);
