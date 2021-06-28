#ifndef _CRON_HPP_
#define _CRON_HPP_

#include <vector>
#include "task.hpp"

class Cron
{
 public:
	void add_task(Task& task);
	bool remove_task(int id);
	std::vector<Task> get_tasks();
	void exit();
 private:
	std::vector<Task> tasks;
};

#endif //_CRON_HPP_
