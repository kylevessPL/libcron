#ifndef _INVALID_ARGS_EXCEPTION_HPP_
#define _INVALID_ARGS_EXCEPTION_HPP_

#include <string>
#include <stdexcept>

class InvalidArgsException : public std::runtime_error
{
 public:
	InvalidArgsException()
		: runtime_error("Invalid arguments, run cron with --help flag to list all available commands")
	{
	}
};

#endif //_INVALID_ARGS_EXCEPTION_HPP_
