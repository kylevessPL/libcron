#include <spawn.h>
#include <algorithm>
#include <cstring>
#include "task.hpp"
#include "utils.hpp"

Task::Task(std::vector<std::string> command, Time execution_time, bool rel, Time period_time)
	: id(TASK_COUNT++), execution_time(execution_time), rel(rel), period_time(period_time), command(command)
{
}

void Task::schedule()
{
	struct itimerspec timer_time;
	timer_time.it_value.tv_sec = this->rel ? this->execution_time.get_total_sec()
										   : this->execution_time.get_current_epoch_time();
	timer_time.it_interval.tv_sec = this->period_time.get_total_sec();

	struct sigevent timer_event;
	timer_event.sigev_notify = SIGEV_THREAD;
	timer_event.sigev_notify_function = Task::execute;
	timer_event.sigev_value.sival_ptr = this;

	if (
		timer_create(CLOCK_REALTIME, &timer_event, &timer) == -1 ||
		timer_settime(timer, this->rel ? 0 : TIMER_ABSTIME, &timer_time, nullptr) == -1)
	{
		throw std::runtime_error("There was an error executing task: " + std::string(std::strerror(errno)));
	}
}

void Task::cancel()
{
	if (timer_delete(timer) == -1)
	{
		throw std::runtime_error("There was an error cancelling task: " + std::string(std::strerror(errno)));
	}
}

int Task::get_id()
{
	return this->id;
}

void Task::execute(__sigval_t arg)
{
	auto task = static_cast<Task*>(arg.sival_ptr);

	std::vector<char*> data = Utils::vec_str_to_char_ptr(task->command, task->command.size(), 1);

	pid_t pid;
	if (posix_spawn(&pid, task->command.at(0).data(), nullptr, nullptr, data.data(), nullptr) == -1)
	{
		throw std::runtime_error("There was an error executing task: " + std::string(std::strerror(errno)));
	}
	task->last_execution_time = std::chrono::system_clock::now();
}

std::ostream& operator<<(std::ostream& os, const Task& task)
{
	return os << "Task id: " << task.id << ", last execution time: "
			  << (task.last_execution_time.time_since_epoch().count() == 0 ? "undefined"
																		   : Utils::pretty_print_chrono_time(task.last_execution_time))
			  << ", repeatable: "
			  << (task.period_time.get_total_sec() != 0 ? ("every " + std::string(task.period_time)) : "no");
}
