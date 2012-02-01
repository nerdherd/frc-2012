#ifndef NerdHerd_Logger_
#define NerdHerd_Logger_

#include <string>
#include <stdio.h>
#include <Timer.h>
#include <Task.h>

class Logger;

class LogBase {
protected:
	friend class Logger;
	virtual void _pLog (FILE *f);
	virtual void _pName (FILE *f);
	LogBase *_nextLog;
public:
	virtual std::string name()=0;
	virtual void log(FILE*)=0;
	virtual ~LogBase();
	LogBase(Logger *l);
};



class Logger {
public:
	Logger (const std::string &filePath, float delay);
	~Logger ();
	void log();
	void init();
private:
	Timer startTime;
	std::string FileName;
	float Delay;
	FILE *file;
	LogBase *head;
	Task TaskLog;
	friend class LogBase;
	int TaskLogger();
	static int s_TaskLogger(Logger*);
	bool inited;
};

#endif // NerdHerd_Logger_
