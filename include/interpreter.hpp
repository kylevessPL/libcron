#ifndef _INTERPRETER_HPP_
#define _INTERPRETER_HPP_

#include <string>
#include <vector>
#include <scheduler.hpp>

class Interpreter
{
 public:
	Interpreter();
	std::string interpret(char* command, bool& flag);
 private:
	std::string handle_add_task(std::vector<std::string> args);
	std::string handle_remove_task(std::vector<std::string> args);
	std::string handle_list();
	std::string handle_help();
	std::string handle_exit(bool& flag);
	Time parse_time(std::string time_str);
	std::vector<std::string> split_args(char* command);
	Scheduler scheduler;
};

#endif //_INTERPRETER_HPP_
