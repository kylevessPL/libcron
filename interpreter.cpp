#include <sstream>
#include <algorithm>
#include <iterator>
#include <deque>
#include "interpreter.hpp"
#include "invalid_args_exception.hpp"

Interpreter::Interpreter()
{
}

std::string Interpreter::interpret(char* command, bool& flag)
{
	std::deque<std::string> args = split_args(command);
	if (args.at(0) == "--shutdown")
	{
		return handle_exit(flag);
	}
	else if (args.at(0) == "--add")
	{
		std::vector<std::string> vec(args.begin() + 1, args.end());
		return handle_add_task(vec);
	}
	else if (args.at(0) == "--remove")
	{
		std::vector<std::string> vec(args.begin() + 1, args.end());
		return handle_remove_task(vec);
	}
	else if (args.at(0) == "--list")
	{
		return handle_list();
	}
	else if (args.at(0) == "--help")
	{
		return handle_help();
	}
	throw InvalidArgsException();
}

std::string Interpreter::handle_add_task(std::vector<std::string>& args)
{
	if (args.empty())
	{
		throw InvalidArgsException();
	}

	Time* period_time;
	bool rel;
	auto it = args.begin();

	if (*it == "-r")
	{
		rel = true;
	}
	it++;
	if (it == args.end())
	{
		throw InvalidArgsException();
	}
	Time execution_time = parse_time(*it);
	it++;
	if (it == args.end())
	{
		throw InvalidArgsException();
	}
	if (*it == "-o")
	{
		it++;
		if (it == args.end())
		{
			throw InvalidArgsException();
		}
		Time time = parse_time(*it);
		period_time = &time;
	}
	it++;
	if (it == args.end())
	{
		throw InvalidArgsException();
	}
	Task task = Task(std::vector<std::string>(it, args.end()), execution_time, rel, std::optional<Time>(*period_time));
	this->scheduler.add_task(task);
	return "Task scheduled successfully";
}

std::string Interpreter::handle_remove_task(std::vector<std::string>& args)
{
	int id;
	try
	{
		id = std::stoi(args.at(1));
	}
	catch (std::exception e)
	{
		throw InvalidArgsException();
	}
	if (!scheduler.remove_task(id))
	{
		return "Task " + std::to_string(id) + " not exists";
	}
	return "Task " + std::to_string(id) + " cancelled";
}

std::string Interpreter::handle_list()
{
	std::vector<Task> tasks = scheduler.get_tasks();
	if (tasks.empty())
	{
		return "No tasks scheduled";
	}
	std::stringstream out;
	std::for_each(tasks.begin(), tasks.end(), [&out](Task task) -> void
	{ out << &task << std::endl; });
	return out.str();
}

std::string Interpreter::handle_help()
{
	std::stringstream out;
	out << "Scheduler: start Scheduler" << std::endl;
	out << "Scheduler --shutdown: remove_task all tasks and shutdown Scheduler daemon" << std::endl;
	out
		<< "Scheduler --add [-r] <h.m.s> [-o <h.m.s>] path [args...]: add_task new Task at specified Time followed by optional offset"
		<< std::endl;
	out << "Scheduler --remove <id>: remove_task Task by id" << std::endl;
	out << "Scheduler --list: list all scheduled tasks" << std::endl;
	out << "Scheduler --help: list available commands" << std::endl;
	return out.str();
}

std::string Interpreter::handle_exit(bool& flag)
{
	scheduler.exit();
	flag = 1;
	return "Scheduler daemon shut down";
}

std::deque<std::string> Interpreter::split_args(char* command)
{
	std::stringstream ss(command);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::deque<std::string> args(begin, end);
	std::copy(args.begin(), args.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
	args.pop_front();
	return args;
}

Time Interpreter::parse_time(std::string time_str)
{
	int hour;
	int min;
	int sec;
	if (std::sscanf(time_str.data(), "%u.%u.%u", &hour, &min, &sec) != 3)
	{
		throw InvalidArgsException();
	}
	return Time(hour, min, sec);
}
