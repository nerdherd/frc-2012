#include "drive.h"

drive::drive(CSVReader *c, Logger *l):
	LogBase(l),
	config(c),
	leftPass(&alpha),
	rightPass(&alpha),
	leftEncoder(3,4),
	rightEncoder(5,6)
{
	scale = config->GetValue("turnScale");
	highSpeed = config->GetValue("driveHigh");
	lowSpeed = config->GetValue("driveLow");
	alpha = config->GetValue("driveAlpha");
	kP = config->GetValue("driveP");
	kI = config->GetValue("driveI");
	kD = config->GetValue("driveD");
	
	left1 = new JaguarLog(l,2);
	left2 = new JaguarLog(l,3);
	right1 = new JaguarLog(l,4);
	right2 = new JaguarLog(l,5);
	
	leftOut = new PIDout(left1, left2);
	rightOut = new PIDout(right1, right2);

	leftEncoder.SetDistancePerPulse(.004);
	leftEncoder.SetPIDSourceParameter(Encoder::kRate);
	leftEncoder.Start();
	rightEncoder.SetDistancePerPulse(.004);
	rightEncoder.SetPIDSourceParameter(Encoder::kRate);
	rightEncoder.Start();
		
	leftPID = new PIDController(kP, kI, kD,
								dynamic_cast<PIDSource*>(&leftEncoder), dynamic_cast<PIDOutput*>(leftOut));
	leftPID->SetSetpoint(0);
	rightPID = new PIDController(kP, kI, kD,
								dynamic_cast<PIDSource*>(&rightEncoder), dynamic_cast<PIDOutput*>(rightOut));
	rightPID->SetSetpoint(0);

}

drive::~drive(){
	delete leftPID;
	delete rightPID;
	delete leftOut;
	delete rightOut;
}

void drive::fix(float &left, float &right){
	float diff = left - right; 
	float speed = (left+right)/2;
	diff*=scale;
	left=speed+diff;
	right=speed-diff; 
}
/*
void drive::lowpass(float &left, float &right){
	
}

void drive::fullfast(float &left, float &right){
	left = max;
	right = max; 
}

void drive::fullslow(float &left, float &right){
	left = min;
	right = min;
}
*/

drive::PIDout::PIDout (JaguarLog *a, JaguarLog *b) : motor1(a), motor2(b) {}
void drive::PIDout::PIDWrite (float output) {
	motor1->Set(output);
	motor2->Set(output);
}

float drive::lowPass::operator () (float value)  {
	return lastValue = lastValue + (value - lastValue) * (*alpha);
}

void drive::reload(){
	scale = config->GetValue("turnScale");
	highSpeed = config->GetValue("driveHigh");
	lowSpeed = config->GetValue("driveLow");
	alpha = config->GetValue("driveAlpha");
	kP = config->GetValue("driveP");
	kI = config->GetValue("driveI");
	kD = config->GetValue("driveD");

	leftPID->Reset();
	rightPID->Reset();
	leftPID->SetPID(kP,kI,kD);
	rightPID->SetPID(kP,kI,kD);
	leftPID->Enable();
	rightPID->Enable();
}

std::string drive::name () {
	return "somethign";
}

void drive::log(FILE *f) {
	fprintf(f, ",%f", scale);
}

