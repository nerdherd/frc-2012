#include "Logger.h"
#include "WPILib.h"



using namespace std;

class LogEnd : protected LogBase {
protected:
	friend class Logger;
	virtual void _pLog(FILE *f) {}
	virtual void _pName(FILE *f) {}
public:
	virtual string name () {return "\n\r"; };
	virtual void log(FILE *f) {
		fprintf(f, "\n\r");
	}
};

Logger::Logger (const string& fileName, float time):
FileName(fileName), Time(time) {
	file = fopen(FileName.c_str(), "a");
	head = new LogEnd();
}

void Logger::init() {
	fprintf(file, "time");
	head->_pName(file);
}

void Logger::log() {
	fprintf(file, "0"); // TODO: show the time since it was turned on or something like that
	head->_pLog(file);
}

void LogBase::_pLog(FILE *f) {
	log(f);
	_nextLog->_pLog(f);
}

void LogBase::_pName(FILE *f) {
	fprintf(f, ",%s", name().c_str());
}
