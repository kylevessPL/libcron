#include <algorithm>
#include <cstring>
#include <spawn.h>
#include <task.hpp>
#include <utils.hpp>

Task::Task(std::vector<std::string> command, Time execution_time, bool rel, Time period_time)
	: id(TASK_COUNT++), execution_time(execution_time), rel(rel), period_time(period_time), command(command)
{
	logger.init(LOG_PATH);
}

void Task::schedule()
{
	struct sigevent timer_event = { 0 };
	timer_event.sigev_notify = SIGEV_THREAD;
	timer_event.sigev_notify_function = Task::callback;
	timer_event.sigev_value.sival_ptr = this;

	struct itimerspec timer_time = { 0 };
	timer_time.it_value.tv_sec = this->rel ? this->execution_time.get_total_sec()
										   : this->execution_time.get_current_epoch_time();
	timer_time.it_interval.tv_sec = this->period_time.get_total_sec();

	if (
		timer_create(CLOCK_REALTIME, &timer_event, &this->timer) == -1 ||
		timer_settime(this->timer, this->rel ? 0 : TIMER_ABSTIME, &timer_time, nullptr) == -1)
	{
		throw std::runtime_error("There was an error scheduling task: " + std::string(std::strerror(errno)));
	}

	logger.log(Logger::Severity::standard, "New task scheduled");
}

void Task::cancel()
{
	if (timer_delete(timer) == -1)
	{
		throw std::runtime_error("There was an error cancelling task: " + std::string(std::strerror(errno)));
	}

	logger.log(Logger::Severity::standard, "Task " + std::to_string(this->get_id()) + " cancelled");
}

void Task::execute()
{
	std::vector<char*> data = Utils::vec_str_to_char(command);

	pid_t pid;
	if (posix_spawn(&pid, this->command.at(0).data(), nullptr, nullptr, data.data(), nullptr) != 0)
	{
		throw std::runtime_error("There was an error executing task: " + std::string(std::strerror(errno)));
	}

	this->last_execution_time = std::chrono::system_clock::now();
	logger.log(Logger::Severity::standard, "Task " + std::to_string(this->get_id()) + " executed");
}

int Task::get_id()
{
	return this->id;
}

void Task::callback(sigval_t arg)
{
	auto task = reinterpret_cast<Task*>(arg.sival_ptr);
	task->execute();
}

std::ostream& operator<<(std::ostream& os, const Task& task)
{
	return os << "Task id: " << task.id << ", last execution time: "
			  << (task.last_execution_time.time_since_epoch().count() == 0 ? "undefined"
																		   : Utils::pretty_print_chrono_time(task.last_execution_time))
			  << ", repeatable: "
			  << (task.period_time.get_total_sec() != 0 ? ("every " + std::string(task.period_time)) : "no");
}
