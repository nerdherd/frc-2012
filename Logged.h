#ifndef NerdHerd_Logged_
#define NerdHerd_Logged_

#include "Logger.h"
#include "WPILib.h"

class JaguarLog : public LogBase, public CANJaguar {
public:
	JaguarLog(Logger *l, UINT8 deviceNumber, CANJaguar::ControlMode controlMode = CANJaguar::kPercentVbus);
	std::string name();
	void log(FILE* f);
};

#endif // NerdHerd_Logged_
