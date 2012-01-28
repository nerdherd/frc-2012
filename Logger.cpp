#include "Logger.h"
#include "WPILib.h"



using namespace std;

class LogEnd : protected LogBase {
protected:
	friend class Logger;
	virtual void _pLog(FILE *f) {
		fprintf(f, "\n\r");
	}
	virtual void _pName(FILE *f) {
		fprintf(f, "\n\r");
	}
public:
	virtual string name () {};
	virtual void log(FILE *f) {}
	LogEnd(): LogBase(NULL) {}
};

Logger::Logger (const string& fileName, float delay):
FileName(fileName), Delay(delay), TaskLog("Logger", (FUNCPTR)s_TaskLogger){
	file = fopen(FileName.c_str(), "w");
	//file = stdout;
	head = new LogEnd();
	startTime.Start();
	TaskLog.Start((int)this);
}

int Logger::s_TaskLogger(Logger *thisPtr)
{
	return thisPtr->TaskLogger();
}

int Logger::TaskLogger() {
	while(true) {
		Wait(Delay);
		log();
	}
}

Logger::~Logger () {
	TaskLog.Stop();
	fclose(file);
	delete head; // should I delete it or let the other systems delete the logging interface
}

void Logger::init() {
	fprintf(file, "time");
	head->_pName(file);
}

void Logger::log() {
	fprintf(file, "%f", startTime.Get()); // TODO: show the time since it was turned on or something like that
	head->_pLog(file);
	fflush(file);
}

void LogBase::_pLog(FILE *f) {
	fprintf(f, ",");
	log(f);
	_nextLog->_pLog(f);
}

void LogBase::_pName(FILE *f) {
	fprintf(f, ",%s", name().c_str());
}

LogBase::~LogBase () {
	delete _nextLog;
}

LogBase::LogBase(Logger *l) {
	_nextLog = l->head;
	l->head = this;
}
