#include "Logger.h"
#include "WPILib.h"



using namespace std;

Logger::Logger (const string& fileName, float time):
FileName(fileName), Time(time) {
	file = fopen(FileName.c_str(), "a");
}
