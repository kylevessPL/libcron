#ifndef _TIME_HPP_
#define _TIME_HPP_

#include <ostream>

class Time
{
 public:
	Time(int hour, int min, int sec);
	int get_current_epoch_time() const;
	int get_total_sec() const;
	operator std::string() const;
 private:
	int hour;
	int min;
	int sec;
	friend std::ostream& operator<<(std::ostream& os, const Time& time);
};

#endif //_TIME_HPP_
