#include "drive.h"

Drive::Drive(Logger *l, CSVReader *c):
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
								dynamic_cast<PIDSource*>(&leftEncoder), dynamic_cast<PIDOutput*>(leftOut), .02);
	leftPID->SetSetpoint(0);
	rightPID = new PIDController(kP, kI, kD,
								dynamic_cast<PIDSource*>(&rightEncoder), dynamic_cast<PIDOutput*>(rightOut), .02);
	rightPID->SetSetpoint(0);
}

Drive::~Drive(){
	delete leftPID;
	delete rightPID;
	delete leftOut;
	delete rightOut;
}

void Drive::fix(float &left, float &right){
	float diff = left - right; 
	float speed = (left+right)/2;
	diff*=scale;
	left=speed+diff;
	right=speed-diff; 
}

Drive::PIDout::PIDout (JaguarLog *a, JaguarLog *b) : motor1(a), motor2(b) {}
void Drive::PIDout::PIDWrite (float output) {
	motor1->Set(output);
	motor2->Set(output);
}

float Drive::lowPass::operator () (float value)  {
	return lastValue = lastValue + (value - lastValue) * (*alpha);
}

void Drive::reload(){
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

std::string Drive::name () {
	return "somethign";
}

void Drive::log(FILE *f) {
	fprintf(f, ",%f", scale);
}

void Drive::run (float left, float right) {
	// square the values the make it more natural
	// then low pass the values
	left = leftPass(left * left);
	right = rightPass(right * right);
	// scale the value for the turning to make it more controllable
	fix(left, right);
	// set teh values to the pid
	leftPID->SetSetpoint(left);
	rightPID->SetSetpoint(right);
}

