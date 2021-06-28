#include <mqueue.h>
#include <unistd.h>
#include <functional>
#include <sys/mman.h>
#include <sys/wait.h>
#include <thread>
#include "server.hpp"
#include "query.hpp"

void Server::start()
{
	Interpreter interp;

	struct mq_attr attr;

	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof(struct query_t);
	attr.mq_flags = 0;

	auto query_queue = static_cast<int>(mq_open("/mq_queries_queue", O_CREAT | O_RDONLY, 0444, &attr));

	handle_query(interp, query_queue);

	int status;
	do
	{
		wait(&status);
	} while (!WIFEXITED(status));

	mq_close(query_queue);
	mq_unlink("mq_queries_queue");
}

void Server::handle_query(Interpreter interp, mqd_t query_queue)
{
	std::thread t([&interp, &query_queue]()
	{
	  while (true)
	  {
		  struct query_t query;

		  mq_receive(query_queue, (char*) &query, sizeof(struct query_t), NULL);

		  auto shm_id = static_cast<int>(shm_open(query.shm_name, O_RDWR, 0777));
		  auto command = static_cast<char*>(mmap(0, sizeof(query.shm_size),
			  PROT_READ | PROT_WRITE, MAP_SHARED, shm_id, 0));

		  std::string response = interp.interpret(command);

		  munmap(command, sizeof(command));
		  close(shm_id);

		  auto response_queue = static_cast<mqd_t>(mq_open(query.response_queue_name, O_WRONLY));

		  mq_send(response_queue, response.data(), response.size(), 0);
		  mq_close(response_queue);
	  }
	});
	t.join();
}
