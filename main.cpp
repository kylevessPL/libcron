#include <iostream>
#include <mqueue.h>
#include <server.hpp>
#include <client.hpp>
#include <logger.hpp>
#include <utils.hpp>
#include <invalid_args_exception.hpp>
#include <lock.hpp>

int main(int argc, char** argv)
{
	Logger logger;
	try
	{
		logger.init(LOG_PATH);
	}
	catch (std::runtime_error e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	try
	{
		bool lock_status = Lock::is_locked(PIDFILE_PATH);

		if (argc <= 1)
		{
			if (lock_status)
			{
				std::cerr << "Cron service already running" << std::endl;
				return EXIT_FAILURE;
			}

			Lock::create_lock(PIDFILE_PATH);
			mq_unlink("/mq_queries_queue");
			std::cout << "Cron service running" << std::endl;
			logger.log(Logger::Severity::min, "Cron service started");

			Server::start();
		}
		else
		{
			if (!lock_status)
			{
				std::cerr << "Cron service not yet running" << std::endl;
				return EXIT_FAILURE;
			}

			std::string res = Client::start(argc, argv);
			std::cout << res << std::endl;
			logger.log(Logger::Severity::min, res);
		}
	}
	catch (InvalidArgsException e)
	{
		std::cerr << e.what() << std::endl;
		logger.log(Logger::Severity::standard, e.what());
		return EXIT_FAILURE;
	}
	catch (std::runtime_error e)
	{
		logger.log(Logger::Severity::max, e.what());
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
