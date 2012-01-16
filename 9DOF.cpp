#include "9DOF.h"

IMU::IMU () {
	DigitalModule *sideCar = DigitalModule::GetInstance(1);
	accel = new ADXL345_I2C(1);
	update();
}

void IMU::update () {
	debug(accel->GetAcceleration(ADXL345_I2C::kAxis_X));
	debug(accel->GetAcceleration(ADXL345_I2C::kAxis_Y));
	debug(accel->GetAcceleration(ADXL345_I2C::kAxis_Z));
}


/*
void IMU_sensors::Accel::update() {
	UINT8 dat[6];
	sensor->Read(0x32, 6, dat);
	debug((int)*dat);
}
*/