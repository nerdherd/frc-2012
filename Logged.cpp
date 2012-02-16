#include "Logged.h"

JaguarLog::JaguarLog (Logger *l, UINT8 deviceNumber, CANJaguar::ControlMode controlMode)
:LogBase(l), CANJaguar(deviceNumber, controlMode)
{
	
}

std::string JaguarLog::name() {
	char num[10];
	sprintf(num, "%d", m_deviceNumber);
	string s = "Jag ";
	s += num;
	s += " value,Jag ";
	s += num;
	s += " voltage,";
	s += "Jag ";
	s += num;
	s += " current";
	if(m_controlMode == CANJaguar::kSpeed) {
		s += ",Jag ";
		s += num;
		s += " speed";
	}else if(m_controlMode == CANJaguar::kPosition) {
		s += ",Jag ";
		s += num;
		s += " position";
	}
	// nothing else
	return s;
}

void JaguarLog::log(FILE *f) {
	double state=-1;
	if(m_controlMode == CANJaguar::kSpeed) {
		state = GetSpeed();
	}else if(m_controlMode == CANJaguar::kPosition) {
		state = GetPosition();
	}
	if(state == -1)
		fprintf(f, ",%f,%f,%f", Get(), GetOutputVoltage(), GetOutputCurrent());
	else
		fprintf(f, ",%f,%f,%f,%f", Get(), GetOutputVoltage(), GetOutputCurrent(), state);
}