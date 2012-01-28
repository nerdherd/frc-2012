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
	s += " voltage,";
	s += "Jag ";
	s += num;
	s += " current,";
	if(m_controlMode == CANJaguar::kSpeed) {
		s += "Jag ";
		s += num;
		s += " speed";
	}else{
		s += "null,";
	}
	return s;
}

void JaguarLog::log(FILE *f) {
	double state=0;
	//if(m_controlMode == CANJaguar::kSpeed) {
		state = Get();
	//}
	fprintf(f, "%f,%f,%f", GetOutputVoltage(), GetOutputCurrent(), state);
}