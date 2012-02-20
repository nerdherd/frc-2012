#include "shooter.h"
#include <math.h>


Shooter::Shooter (CSVReader *c, Logger *l, CameraTracking *cam) :
	LogBase(l),
	config(c) ,
	camera(cam),
	encoder(1,2),
	distance(2),
	TaskShoot("Shooter", (FUNCPTR)s_TaskShoot),
	LimitTurret(3),	
	inited(false),
	previousDistance(-1)
{
	motor1 = new JaguarLog(l, 6);
	motor2 = new JaguarLog(l, 7);
	encoder.SetDistancePerPulse(.004);
	encoder.SetPIDSourceParameter(Encoder::kRate);
	encoder.Start();
	pid = new PIDController(1,0,0, // reread from the config file at the end of this function
							dynamic_cast<PIDSource*>(&encoder), dynamic_cast<PIDOutput*>(this),.05);
	pid->SetSetpoint(0);
	turret = new JaguarLog(l, 8);//, CANJaguar::kPosition);
	
//	reload();
}

float Shooter::computeDistance () {
	// return the distance in inch
	float weight, sum;
	sum = distance.GetVoltage()/.0098 * .3;
	weight = .3;
	if(previousDistance != -1) {
		sum += previousDistance * .3;
		weight += .3;
	}
	if(camera->fresh) {
		sum += camera->distance * .5;
		weight += .5;
		camera->fresh=false;
	}
	previousDistance = sum / weight;
	return previousDistance;
}

float Shooter::computeSpeed (float d) {
	// return the speed in RPM
	
	// this is just as hackish this gets for now, but if it works then lol
	return d*10;
	//return config->GetValue("shooter");
}

void Shooter::computeTurn () {
	if(camera->targets.empty()) {
		if(TurretLocation < .05 || TurretLocation > .95) TurretSweepDir = !TurretSweepDir;
		TurretLocation += TurretSweepDir ? .02 : -.02; // should take about 3 seconds to turn all the way around in search of the larget
	}else{
		if(camera->fresh)
			TurretLocation = turret->GetPosition() + camera->horizantal * .175; // 42/240 (calculated deg for turning)
	}
}

void Shooter::reload () {
	
	pid->SetSetpoint(0);
	pid->Reset();
	pid->SetPID(config->GetValue("shooterP"), config->GetValue("shooterI"), config->GetValue("shooterD"));
	if(inited == false) {
		motor1->Set(0);
		motor2->Set(0);
		int count=50;
		turret->Set(-.3);
		while(--count > 0 && LimitTurret.Get() == 1) Wait(.005);
		turret->Set(.2);
		while(--count > 0 && LimitTurret.Get() == 0) Wait(.005);
		turret->Set(0);
		turret->ChangeControlMode(CANJaguar::kPosition);
		turret->EnableControl(0);
		inited=true;
		TaskShoot.Start((int)this);
	}else{
		// if this doesn't work then change this to take what ever the last target value was
		turret->EnableControl(turret->GetPosition());
	}
	// set this to be based off the number of complete turns of the whole turret
	turret->ConfigEncoderCodesPerRev((int)(500 * config->GetValue("turretMotorTurns"))); 
	turret->SetPID(config->GetValue("turretP"), config->GetValue("turretI"), config->GetValue("turretD"));
	pid->Enable();
}

void Shooter::s_TaskShoot  (Shooter *self) {
	self->run();
}

void Shooter::run () {
	while(true) {
		computeTurn();
		// for the speed of the shooter
		pid->SetSetpoint(-1*computeSpeed(computeDistance())/60.);
		printf("encoder %f\n", encoder.GetRate()*60);
		
		double intPart;
		turret->Set(modf(TurretLocation, &intPart));
		if(LimitTurret.Get() == 1) {
			// I am rezeroing the turret because it might slip on the turning and this will reset it as it should be
			turret->EnableControl(0);
			// I think that this has to get reset to the jaguar after it is enabled
			turret->ConfigEncoderCodesPerRev((int)(500 * config->GetValue("turretMotorTurns"))); 
			turret->SetPID(config->GetValue("turretP"), config->GetValue("turretI"), config->GetValue("turretD"));
		}
		
		Wait(.05);
	}
}

Shooter::~Shooter() {
	// shooter1 and shooter2 will be deleted by the logger
	pid->Disable();
	delete pid;
}


void Shooter::PIDWrite(float output) {
	if(output > 0) output = 0;
	motor1->Set(output);
	motor2->Set(output);
}


std::string Shooter::name () {
	return "shooter speed,shooter target,believed distance";
}

void Shooter::log(FILE *f) {
	fprintf(f, ",%f,%f,%f", encoder.GetRate(), pid->Get(), previousDistance);
}
