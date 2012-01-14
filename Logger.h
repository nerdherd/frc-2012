#ifndef NerdHerd_Logger_
#define NerdHerd_Logger_

#include <string>
#include <stdio.h>


class Logger {
public:
	Logger (const std::string &filePath, float time);
private:
	std::string FileName;
	float Time;
	FILE *file;
};

#endif // NerdHerd_Logger_
