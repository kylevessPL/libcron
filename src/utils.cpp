#include <algorithm>
#include <utils.hpp>

std::string Utils::pretty_print_chrono_time(const std::chrono::time_point<std::chrono::system_clock> time_point)
{
	std::time_t t = std::chrono::system_clock::to_time_t(time_point);
	return std::ctime(&t);
}

std::vector<char*> Utils::vec_str_to_char(std::vector<std::string> vec)
{
	std::vector<char*> arr;
	for (int i = 0; i < vec.size(); ++i)
	{
		arr.push_back(vec[i].data());
	}
	arr.push_back(nullptr);
	return arr;
}
