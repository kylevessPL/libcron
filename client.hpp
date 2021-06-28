#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_

#include "query.hpp"

class Client
{
 public:
	static std::string start(int size, char** args);
 private:
	static std::string join_arguments(int size, char** args);
};

#endif //_CLIENT_HPP_
