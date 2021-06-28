#include <sstream>
#include <algorithm>
#include <iterator>
#include <deque>
#include "interpreter.hpp"
#include "invalid_args_exception.hpp"

Interpreter::Interpreter()
{
}

std::string Interpreter::interpret(char* command)
{
	std::deque<std::string> args = split_args(command);
	if (args.at(0) == "--shutdown")
	{
		return handle_exit();
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
	return "Unknown command, run Cron with --help flag to list all available commands";
}

std::string Interpreter::handle_add_task(std::vector<std::string>& args)
{
	return std::__cxx11::string();
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
	if (!cron.remove_task(id))
	{
		return "Task " + std::to_string(id) + " not exists";
	}
	return "Task " + std::to_string(id) + " cancelled";
}

std::string Interpreter::handle_list()
{
	std::vector<Task> tasks = cron.get_tasks();
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
	out << "Cron: start Cron" << std::endl;
	out << "Cron --shutdown: remove_task all tasks and shutdown Cron daemon" << std::endl;
	out
		<< "Cron --add [-r] <h.m.s> [-o <h.m.s>] path [args...]: add_task new Task at specified Time followed by optional offset"
		<< std::endl;
	out << "Cron --remove <id>: remove_task Task by id" << std::endl;
	out << "Cron --list: list all scheduled tasks" << std::endl;
	out << "Cron --help: list available commands" << std::endl;
	return out.str();
}

std::string Interpreter::handle_exit()
{
	cron.exit();
	return "Cron daemon shut down";
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
