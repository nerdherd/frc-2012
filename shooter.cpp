#include "shooter.h"


Shooter::Shooter (CSVReader *c, Logger *l) :
	LogBase(l),
	config(c) ,
	encoder(1,2)
{
	motor1 = new JaguarLog(l, 5);
	motor2 = new JaguarLog(l, 7);
	encoder.SetDistancePerPulse(.004);
	encoder.SetPIDSourceParameter(Encoder::kRate);
	encoder.Start();
	pid = new PIDController(config->GetValue("shooterP"), config->GetValue("shooterI"), config->GetValue("shooterD"),
							dynamic_cast<PIDSource*>(&encoder), dynamic_cast<PIDOutput*>(this),.05);
	//pid->SetOutputRange(0,1.0);
	pid->Enable();
}

void Shooter::reload () {
	pid->Reset();
	pid->SetPID(config->GetValue("shooterP"), config->GetValue("shooterI"), config->GetValue("shooterD"));
	pid->Enable();
}

void Shooter::run () {
	pid->SetSetpoint(-1*config->GetValue("shooter")/60.);
	printf("encoder %f\n", encoder.GetRate());
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
	return "shooter";
}

void Shooter::log(FILE *f) {
	fprintf(f, ",0");
}
