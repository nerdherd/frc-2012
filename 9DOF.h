#ifndef NerdHerd_9DOF_
#define NerdHerd_9DOF_

#include "WPILib.h"
#include <iostream>
#define debug(x) \
	std::cerr << __LINE__ << ": " << x << std::endl;

namespace IMU_sensors {
	
}

class IMU {
private:
	ADXL345_I2C *accel;
public:
	IMU ();
	void update();
};

#endif // NerdHerd_9DOF_
