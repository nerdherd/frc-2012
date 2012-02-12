#include "9DOF.h"

IMU::IMU (Logger *log):
LogBase(log),
IMUTask("9DOF", (FUNCPTR)s_IMUTask, 103) {
	DigitalModule *sideCar = DigitalModule::GetInstance(1);
	test = sideCar->GetI2C(0x3A);
	//accel = new ADXL345_I2C(1);
	
	IMUTask.Start((int)this);
}

void IMU::s_IMUTask (IMU* self) {
	Wait(5);
	self->TaskFunction();
}

void IMU::TaskFunction () {
	while(true) {
		update();
		Wait(.1);
	}
}

void IMU::update () {
	printf("running\n");
	if(test->AddressOnly()) {
		printf("working\n");
	}else{
		printf("failure\n");
	}
	//printf("X %f\n", accel->GetAcceleration(ADXL345_I2C::kAxis_X));
	//std::cout << accel->GetAcceleration(ADXL345_I2C::kAxis_X) << std::endl;
	//debug(accel->GetAcceleration(ADXL345_I2C::kAxis_X));
	//debug(accel->GetAcceleration(ADXL345_I2C::kAxis_Y));
	//debug(accel->GetAcceleration(ADXL345_I2C::kAxis_Z));
}


/*
void IMU_sensors::Accel::update() {
	UINT8 dat[6];
	sensor->Read(0x32, 6, dat);
	debug((int)*dat);
}
*/

std::string IMU::name () {
	return "9DOF";
}

void IMU::log(FILE *f) {
	fprintf(f, ",0");
}