#include <algorithm>
#include "utils.hpp"

std::string Utils::pretty_print_chrono_time(const std::chrono::time_point<std::chrono::system_clock>& time_point)
{
	std::time_t t = std::chrono::system_clock::to_time_t(time_point);
	return std::ctime(&t);
}

std::vector<char*> Utils::vec_str_to_char_ptr(std::vector<std::string> vec, unsigned int to, unsigned int from)
{
	std::vector<char*> data;
	data.resize(to - from, nullptr);
	std::transform(vec.begin() + from, vec.end() - (vec.size() - to), std::begin(data), [&](const std::string& str)
	{
	  return str.data();
	});
	return data;
}
