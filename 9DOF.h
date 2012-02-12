#ifndef NerdHerd_9DOF_
#define NerdHerd_9DOF_

#include "WPILib.h"
#include <iostream>
#include "Logger.h"
#define debug(x) \
	std::cerr << __LINE__ << ": " << x << std::endl;

namespace IMU_sensors {
	
}

class IMU : public LogBase {
private:
	ADXL345_I2C *accel;
	I2C *test;
	Task IMUTask;
	static void s_IMUTask (IMU *self);
	void TaskFunction();
	
public:
	IMU (Logger *log);
	std::string name();
	void log(FILE *f);
	void update();
};

#endif // NerdHerd_9DOF_
