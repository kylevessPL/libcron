#ifndef _TASK_HPP_
#define _TASK_HPP_

#include <iostream>
#include <optional>
#include <chrono>
#include <vector>
#include <time.h>
#include <csignal>
#include <time.hpp>

class Task
{
 public:
	Task(std::vector<std::string> command, Time execution_time, bool rel, Time period_time);
	void schedule();
	void cancel();
	void execute();
	int get_id();
 private:
	int id;
	Time execution_time;
	Time period_time;
	bool rel;
	std::vector<std::string> command;
	timer_t timer;
	static void callback(sigval_t arg);
	std::chrono::time_point<std::chrono::system_clock> last_execution_time;
	friend std::ostream& operator<<(std::ostream& os, const Task& task);
};

inline static int TASK_COUNT = 1;

#endif //_TASK_HPP_
