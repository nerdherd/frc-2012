#include "drive.h"

#include <iostream>
using namespace std;

Drive::Drive(Logger *l, CSVReader *c):
	LogBase(l),
	config(c),
	leftPass(&alpha),
	rightPass(&alpha),
	speedPass(&speedChangeAlpha),
	leftEncoder(3,4),
	rightEncoder(1,2),
	driveShiftUp(1),
	driveShiftDown(2),
	shiftMode(true)
{
	scale = config->GetValue("turnScale");
	//highSpeed = config->GetValue("driveHigh");
	//lowSpeed = config->GetValue("driveLow");
	alpha = config->GetValue("driveAlpha");
	kP = config->GetValue("driveP");
	kI = config->GetValue("driveI");
	kD = config->GetValue("driveD");
	speedChangeAlpha = config->GetValue("driveSpeedAlpha");
	
	left1 = new JaguarLog(l,2);
	left2 = new JaguarLog(l,3);
	right1 = new JaguarLog(l,4);
	right2 = new JaguarLog(l,5);
	
	leftOut = new PIDout(left1, left2, &leftPass);
	rightOut = new PIDout(right1, right2, &rightPass);

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

Drive::PIDout::PIDout (JaguarLog *a, JaguarLog *b, lowPass *l) : motor1(a), motor2(b), filter(l) {}
void Drive::PIDout::PIDWrite (float output) {
	output = (*filter)(output);
	motor1->Set(output);
	motor2->Set(output);
}

float Drive::lowPass::operator () (float value)  {
	lastValue = lastValue + (value - lastValue) * (*alpha);
	if(lastValue < .05 && lastValue > -.05)
		return 0;
	return lastValue;
}

void Drive::reload(){
	scale = config->GetValue("turnScale");
	currentSpeed = config->GetValue("driveHigh") / 60.;
	//lowSpeed = config->GetValue("driveLow");
	alpha = config->GetValue("driveAlpha");
	speedChangeAlpha = config->GetValue("driveSpeedAlpha");
	kP = config->GetValue("driveP");
	kI = config->GetValue("driveI");
	kD = config->GetValue("driveD");

	leftPID->Reset();
	rightPID->Reset();
	leftPID->SetPID(kP,kI,kD);
	rightPID->SetPID(kP,kI,kD);
	leftPID->Enable();
	rightPID->Enable();
	
	shiftMode = false;
	//driveShift.Set(Relay::kReverse);
	driveShiftDown.Set(false);
	driveShiftUp.Set(true);
}

std::string Drive::name () {
	return "Max speed,Left Set,Left actual,Right Set,Right actual,";
}

void Drive::log(FILE *f) {
	fprintf(f, ",%f,%f,%f,%f,%f", currentSpeed, TargetLeft, leftEncoder.GetRate(), TargetRight, rightEncoder.GetRate());
}

void Drive::run (float left, float right) {
	// square the values the make it more natural
	// then low pass the values
	//left = leftPass(left * left * left); // I am being lazy right now and this will keep the sign
	//right = rightPass(right * right * right);
	left = left * left * left;
	right = right * right * right;
	// scale the value for the turning to make it more controllable
	fix(left, right);
	// set the values to the pid
	//currentSpeed=1;
	//left1->Set(left * currentSpeed);
	//left2->Set(left * currentSpeed);
	//right1->Set(-1 * right * currentSpeed);
	//right2->Set(-1 * right * currentSpeed);
	float maxSpeed = speedPass(currentSpeed);
	TargetLeft = left * maxSpeed;
	TargetRight = right * maxSpeed;
	leftPID->SetSetpoint(TargetLeft);
	rightPID->SetSetpoint(-1 * TargetRight);
}

void Drive::shift(bool s) {
	//if(s==shiftMode) return;
	shiftMode = s;
	if(shiftMode) {
		currentSpeed = config->GetValue("driveHigh") / 60.;
		driveShiftUp.Set(true);
		driveShiftDown.Set(false);
	}else{
		currentSpeed = config->GetValue("driveLow") / 60.;
		driveShiftDown.Set(true);
		driveShiftUp.Set(false);
	}
}