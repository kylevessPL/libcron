#include <spawn.h>
#include <algorithm>
#include <cstring>
#include <csignal>
#include "task.hpp"
#include "utils.hpp"

Task::Task(const std::vector<char*>& command, const Time& execution_time, const bool rel, const std::optional<Time>& period_time)
	: id(task_count++), execution_time(execution_time), period_time(period_time), rel(rel), command(command)
{
}

void Task::schedule()
{
	struct itimerspec timer_time;
	timer_time.it_value.tv_sec = this->rel ? this->execution_time.get_total_sec()
										   : this->execution_time.get_epoch_time();
	timer_time.it_interval.tv_sec = this->period_time->get_total_sec();

	struct sigevent timer_event;
	timer_event.sigev_notify = SIGEV_THREAD;
	timer_event.sigev_notify_function = Task::execute;
	timer_event.sigev_value.sival_ptr = this;

	if (
		timer_create(CLOCK_REALTIME, &timer_event, &timer) == -1 ||
		timer_settime(timer, this->rel ? 0 : TIMER_ABSTIME, &timer_time, nullptr) == -1)
	{
		throw std::runtime_error("There was a problem executing task: " + std::string(std::strerror(errno)));
	}
}

void Task::cancel()
{
	if (timer_delete(timer) == -1)
	{
		throw std::runtime_error("There was a problem cancelling task: " + std::string(std::strerror(errno)));
	}
}

int Task::get_id()
{
	return this->id;
}

void Task::execute(__sigval_t arg)
{
	auto task = static_cast<Task*>(arg.sival_ptr);
	pid_t pid;
	if (posix_spawn(&pid, task->command.at(0), nullptr, nullptr, std::vector<char*>(
		task->command.begin() + 1, task->command.end()).data(), nullptr) == -1)
	{
		throw std::runtime_error("There was a problem executing task: " + std::string(std::strerror(errno)));
	}
	task->last_execution_time = std::chrono::system_clock::now();
}

std::ostream& operator<<(std::ostream& os, const Task& task)
{
	return os << "Task id: " << task.id << ", last execution time: "
			  << (task.last_execution_time.time_since_epoch().count() == 0 ? "undefined"
																		   : Utils::pretty_print_chrono_time(task.last_execution_time))
			  << ", repeatable: "
			  << (task.period_time.has_value() ? ("every " + std::string(task.period_time.value())) : "no");
}