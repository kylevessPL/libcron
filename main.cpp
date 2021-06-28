#include <iostream>
#include <unistd.h>
#include <mqueue.h>
#include "server.hpp"
#include "client.hpp"

int main(int argc, char** argv)
{
	mq_unlink("/mq_queries_queue");

	if (fork() != 0)
	{
		if (argc != 1)
		{
			std::cerr << "Cron daemon not yet running" << std::endl;
			return EXIT_FAILURE;
		}
		try
		{
			Server::start();
		}
		catch (std::runtime_error e)
		{
			std::cerr << "An error occurred: " << e.what() << std::endl;
		}
	}
	else
	{
		if (argc == 1)
		{
			std::cerr << "Cron daemon already running" << std::endl;
			return EXIT_FAILURE;
		}
		try
		{
			Client::start(argc, argv);
		}
		catch (std::runtime_error e)
		{
			std::cerr << "An error occurred: " << e.what() << std::endl;
		}
	}
	return EXIT_SUCCESS;
}
