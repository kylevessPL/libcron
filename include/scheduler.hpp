#ifndef _SCHEDULER_HPP_
#define _SCHEDULER_HPP_

#include <vector>
#include <task.hpp>
#include "logger.hpp"

class Scheduler
{
 public:
	Scheduler();
	void add_task(std::vector<std::string> command, Time execution_time, bool rel, Time period_time);
	bool remove_task(int id);
	std::vector<Task*> get_tasks();
	void exit();
 private:
	Logger logger;
	std::vector<Task*> tasks;
};

#endif //_SCHEDULER_HPP_
