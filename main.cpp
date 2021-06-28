#include <iostream>
#include <unistd.h>
#include <mqueue.h>
#include "server.hpp"
#include "client.hpp"
#include "invalid_args_exception.hpp"
#include "logger.hpp"
#include "utils.hpp"

int main(int argc, char** argv)
{
	Logger logger = Logger(LOG_PATH);

	mq_unlink("/mq_queries_queue");

	if (fork() != 0)
	{
		if (argc != 1)
		{
			std::cerr << "Scheduler daemon not yet running" << std::endl;
			return EXIT_FAILURE;
		}
		try
		{
			Server::start();
		}
		catch (std::runtime_error e)
		{
			logger.log(Logger::Severity::max, e.what());
		}
	}
	else
	{
		if (argc == 1)
		{
			std::cerr << "Scheduler daemon already running" << std::endl;
			return EXIT_FAILURE;
		}
		try
		{
			std::string res = Client::start(argc, argv);
			std::cout << res << std::endl;
			logger.log(Logger::Severity::min, res);
		}
		catch (InvalidArgsException e)
		{
			std::cerr << e.what() << std::endl;
			logger.log(Logger::Severity::standard, e.what());
		}
		catch (std::runtime_error e)
		{
			logger.log(Logger::Severity::max, e.what());
		}
	}
	return EXIT_SUCCESS;
}
