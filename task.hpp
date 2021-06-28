#ifndef _TASK_HPP_
#define _TASK_HPP_

#include <iostream>
#include <optional>
#include <chrono>
#include <vector>
#include <time.h>
#include "time.hpp"

class Task
{
 public:
	Task(const std::vector<std::string>& command, const Time& execution_time, const bool rel, const std::optional<Time>& period_time = std::nullopt);
	void schedule();
	void cancel();
	int get_id();
 private:
	const int id;
	const Time& execution_time;
	const std::optional<Time>& period_time;
	const bool rel;
	const std::vector<std::string>& command;
	timer_t timer;
	static void execute(__sigval_t arg);
	std::chrono::time_point<std::chrono::system_clock> last_execution_time;
	friend std::ostream& operator<<(std::ostream& os, const Task& task);
};

inline static int TASK_COUNT = 1;

#endif //_TASK_HPP_
