#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <iostream>
#include <chrono>
#include <vector>

class Utils
{
 public:
	static std::string pretty_print_chrono_time(const std::chrono::time_point<std::chrono::system_clock> time_point);
	static std::vector<char*> vec_str_to_char_ptr(std::vector<std::string> vec, unsigned int to, unsigned int from = 0);
};

inline static const std::string LOG_PATH = "cron.log";
inline static const std::string PIDFILE_PATH = "/var/run/cron.pid";

#endif //_UTILS_HPP_
