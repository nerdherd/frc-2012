#include "Logger.h"
#include "WPILib.h"



using namespace std;

class LogEnd : protected LogBase {
protected:
	friend class Logger;
	virtual void _pLog(FILE *f) {
		//fprintf(f, "\n");
	}
	virtual void _pName(FILE *f) {
		//fprintf(f, "\n");
	}
public:
	virtual string name () {
		return "";
	}
	virtual void log(FILE *f) {}
	LogEnd(): LogBase(NULL) {}
};

Logger::Logger (const string& fileName, float delay):
FileName(fileName), Delay(delay), TaskLog("Logger", (FUNCPTR)s_TaskLogger, 105, 50000), inited(false){
	file = fopen(FileName.c_str(), "w");
	//file = stdout;
	head = new LogEnd();
	startTime.Start();
	//TaskLog.Start((int)this);
}

int Logger::s_TaskLogger(Logger *thisPtr)
{
	Wait(2.0);
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
	if(!inited) {
		fprintf(file, "time");
		head->_pName(file);
		TaskLog.Start((int)this);
		inited=true;
	}
}

void Logger::log() {
	//cout << "saving log\n";
	fprintf(file, "\n%f", startTime.Get());
	head->_pLog(file);
	fflush(file);
}

void LogBase::_pLog(FILE *f) {
	//fprintf(f, ",");
	log(f);
	_nextLog->_pLog(f);
}

void LogBase::_pName(FILE *f) {
	fprintf(f, ",%s", name().c_str());
	_nextLog->_pName(f); 
}

LogBase::~LogBase () {
	delete _nextLog;
}

LogBase::LogBase(Logger *l) {
	_nextLog = l->head;
	l->head = this;
}
