#include <algorithm>
#include <scheduler.hpp>
#include <utils.hpp>

void Scheduler::add_task(Task& task)
{
	task.schedule();
	this->tasks.push_back(task);
	logger.log(Logger::Severity::standard, "New task added");
}

bool Scheduler::remove_task(int id)
{
	auto it = find_if(this->tasks.begin(), this->tasks.end(), [&id](Task& task)
	{
	  return task.get_id() == id;
	});

	if (it == this->tasks.end())
	{
		return false;
	}
	int idx = std::distance(this->tasks.begin(), it);
	this->tasks.at(idx).cancel();
	this->tasks.erase(it);
	logger.log(Logger::Severity::standard, "Task " + std::to_string(it->get_id()) + " added");
	return true;
}

std::vector<Task> Scheduler::get_tasks()
{
	return this->tasks;
}

void Scheduler::exit()
{
	std::for_each(this->tasks.begin(), this->tasks.end(), [](Task& task) -> void
	{
	  task.cancel();
	  delete &task;
	});
	this->tasks.clear();
	logger.log(Logger::Severity::standard, "All tasks cancelled");
}

Scheduler::Scheduler()
{
	logger.init(LOG_PATH);
}
