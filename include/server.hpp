#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <interpreter.hpp>

class Server
{
 public:
	static void start();
 private:
	static void handle_query(Interpreter interp, mqd_t query_queue);
};

#endif //_SERVER_HPP_
