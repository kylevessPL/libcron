#include <thread>
#include <algorithm>
#include <cstring>
#include <mqueue.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <query.hpp>
#include <client.hpp>
#include <invalid_args_exception.hpp>

std::string Client::start(int size, char** args)
{
	mqd_t query_queue;
	do
	{
		query_queue = mq_open("/mq_queries_queue", O_WRONLY);
		sleep(1);
	} while (query_queue == -1);

	std::string command = join_arguments(size, args);

	struct query_t query;

	sprintf(query.response_queue_name, "/mq_reply_queue_%d", getpid());
	sprintf(query.shm_name, "/shared_memory_%d", getpid());
	query.shm_size = sizeof(command);

	auto shm_id = shm_open(query.shm_name, O_RDWR | O_CREAT, 0777);
	if (shm_id == -1)
	{
		throw std::runtime_error("There was an error initializing shared memory: " + std::string(std::strerror(errno)));
	}

	ftruncate(shm_id, query.shm_size);
	auto query_msg = static_cast<char*>(mmap(0, query.shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_id, 0));
	if (query_msg == MAP_FAILED)
	{
		throw std::runtime_error("There was an error allocating memory: " + std::string(std::strerror(errno)));
	}

	sprintf(query_msg, "%s", command.data());

	mq_send(query_queue, (const char*) &query, sizeof(struct query_t), 0);

	char res[2000];

	struct mq_attr attr;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof(res);
	attr.mq_flags = 0;

	auto response_queue = mq_open(query.response_queue_name, O_CREAT | O_RDONLY, 0644, &attr);
	if (response_queue == -1)
	{
		throw std::runtime_error("There was an error initializing message queue: " + std::string(std::strerror(errno)));
	}

	mq_receive(response_queue, res, sizeof(res), NULL);

	mq_close(query_queue);
	mq_close(response_queue);
	mq_unlink(query.response_queue_name);

	munmap(query_msg, query.shm_size);
	close(shm_id);
	shm_unlink(query.shm_name);

	return res;
}

std::string Client::join_arguments(int size, char** args)
{
	std::string argument;
	std::for_each(args + 1, args + size, [&](const char* c_str)
	{ argument += std::string(c_str) + " "; });
	return argument;
}
