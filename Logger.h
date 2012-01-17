#ifndef NerdHerd_Logger_
#define NerdHerd_Logger_

#include <string>
#include <stdio.h>

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
};



class Logger {
public:
	Logger (const std::string &filePath, float time);
	~Logger ();
	void log();
	void init();
private:
	std::string FileName;
	float Time;
	FILE *file;
	LogBase *head;
};

#endif // NerdHerd_Logger_
