#include <sstream>
#include <chrono>
#include "time.hpp"

Time::Time(int hour, int min, int sec) : hour(hour), min(min), sec(sec)
{
}

int Time::get_epoch_time() const
{
	auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	auto tm = std::localtime(&time);
	tm->tm_hour = this->hour;
	tm->tm_min = this->min;
	tm->tm_sec = this->sec;
	return std::chrono::system_clock::from_time_t(std::mktime(tm)).time_since_epoch().count();
}

int Time::get_total_sec() const
{
	return (this->hour * 60 + this->min) * 60 + this->sec;
}

Time::operator std::string() const
{
	std::ostringstream out;
	out << *this;
	return out.str();
}

std::ostream& operator<<(std::ostream& os, const Time& time)
{
	return os << (time.hour ? (std::to_string(time.hour) + " h ") : "")
			  << (time.min ? (std::to_string(time.min) + " min ") : "")
			  << (time.sec ? (std::to_string(time.sec) + " sec") : "");
}
