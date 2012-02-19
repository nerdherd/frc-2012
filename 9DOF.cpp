#include "9DOF.h"

IMU::IMU (Logger *log):
LogBase(log),
IMUTask("9DOF", (FUNCPTR)s_IMUTask, 103) {
	DigitalModule *sideCar = DigitalModule::GetInstance(1);
	test = sideCar->GetI2C(0x3A);
	test->SetCompatibilityMode(true);
	//accel = new ADXL345_I2C(1);
	
	IMUTask.Start((int)this);
}

IMU::~IMU () {
	delete accel;
}

void IMU::s_IMUTask (IMU* self) {
	Wait(5);
	self->TaskFunction();
}

void IMU::TaskFunction () {
	while(true) {
		update();
		Wait(1.5);
	}
}

void IMU::update () {
	printf("running\n");
	//fflush(stdout);
	if(!test->AddressOnly()) {
		printf("working\n");
	}else{
		printf("failure\n");
	}
	float x = accel->GetAcceleration(ADXL345_I2C::kAxis_X),
		y = accel->GetAcceleration(ADXL345_I2C::kAxis_Y),
		z = accel->GetAcceleration(ADXL345_I2C::kAxis_Z);
	printf("%f %f %f\n", x,y,z);
	
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