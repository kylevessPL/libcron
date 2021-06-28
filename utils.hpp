#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <iostream>
#include <chrono>

class Utils
{
 public:
	static std::string pretty_print_chrono_time(const std::chrono::time_point<std::chrono::system_clock>& time_point);
};

#endif //_UTILS_HPP_
