#include "utils.hpp"

std::string Utils::pretty_print_chrono_time(const std::chrono::time_point<std::chrono::system_clock>& time_point)
{
	std::time_t t = std::chrono::system_clock::to_time_t(time_point);
	return std::ctime(&t);
}
