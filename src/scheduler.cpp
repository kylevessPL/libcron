#include <algorithm>
#include <scheduler.hpp>
#include <utils.hpp>

void Scheduler::add_task(std::vector<std::string> command, Time execution_time, bool rel, Time period_time)
{
	auto task = new Task(command, execution_time, rel, period_time);
	try
	{
		task->schedule();
	}
	catch (std::runtime_error e)
	{
		delete task;
		throw;
	}
	this->tasks.push_back(task);
	logger.log(Logger::Severity::standard, "New task scheduled");
}

bool Scheduler::remove_task(int id)
{
	auto it = find_if(this->tasks.begin(), this->tasks.end(), [&id](Task* task)
	{
	  return task->get_id() == id;
	});

	if (it == this->tasks.end())
	{
		return false;
	}
	int idx = std::distance(this->tasks.begin(), it);
	auto task = this->tasks.at(idx);
	task->cancel();
	this->tasks.erase(it);
	logger.log(Logger::Severity::standard, "Task " + std::to_string(task->get_id()) + " cancelled");
	return true;
}

std::vector<Task*> Scheduler::get_tasks()
{
	return this->tasks;
}

void Scheduler::exit()
{
	std::for_each(this->tasks.begin(), this->tasks.end(), [](Task* task) -> void
	{
	  task->cancel();
	  delete task;
	});
	this->tasks.clear();
	logger.log(Logger::Severity::standard, "All tasks cancelled");
}

Scheduler::Scheduler()
{
	logger.init(LOG_PATH);
}
