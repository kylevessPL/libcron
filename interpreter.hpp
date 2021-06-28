#ifndef _INTERPRETER_HPP_
#define _INTERPRETER_HPP_

#include <string>
#include <vector>
#include "cron.hpp"

class Interpreter
{
 public:
	Interpreter();
	std::string interpret(char* command);
 private:
	std::string handle_add_task(std::vector<std::string>& args);
	std::string handle_remove_task(std::vector<std::string>& args);
	std::string handle_list();
	std::string handle_help();
	std::string handle_exit();
	std::deque<std::string> split_args(char* command);
	Cron cron;
};

#endif //_INTERPRETER_HPP_
