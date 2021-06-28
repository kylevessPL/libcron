#ifndef _SCHEDULER_HPP_
#define _SCHEDULER_HPP_

#include <vector>
#include <task.hpp>

class Scheduler
{
 public:
	void add_task(Task& task);
	bool remove_task(int id);
	std::vector<Task> get_tasks();
	void exit();
 private:
	std::vector<Task> tasks;
};

#endif //_SCHEDULER_HPP_
