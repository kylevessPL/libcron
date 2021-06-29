#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include <string>
#include <mutex>
#include <fstream>

class Logger
{
 public:
	Logger();
	void init(std::string path);
	enum Severity
	{
		min,
		standard,
		max
	};
	void log(Severity sv, std::string msg);
 private:
	std::mutex lock;
	std::ofstream fs;
	std::string get_sv_str(Severity sv);
};

#endif //_LOGGER_HPP_
