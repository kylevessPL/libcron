#include <functional>
#include <thread>
#include <cstring>
#include <mqueue.h>
#include <unistd.h>
#include <sys/mman.h>
#include <server.hpp>
#include <query.hpp>

void Server::start()
{
	struct mq_attr attr;

	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof(struct query_t);
	attr.mq_flags = 0;

	auto query_queue = mq_open("/mq_queries_queue", O_CREAT | O_RDONLY, 0444, &attr);
	if (query_queue == -1)
	{
		throw std::runtime_error("There was an error initializing message queue: " + std::string(std::strerror(errno)));
	}

	handle_query(query_queue);

	mq_close(query_queue);
	mq_unlink("/mq_queries_queue");
}

void Server::handle_query(mqd_t& query_queue)
{
	std::thread t([&query_queue]()
	{
	  Interpreter interp;
	  for (;;)
	  {
		  struct query_t query;

		  mq_receive(query_queue, (char*) &query, sizeof(struct query_t), NULL);

		  auto shm_id = shm_open(query.shm_name, O_RDWR, 0777);
		  if (shm_id == -1)
		  {
			  throw std::runtime_error(
				  "There was an error initializing shared memory: " + std::string(std::strerror(errno)));
		  }

		  auto command = static_cast<char*>(mmap(0, sizeof(query.shm_size),
			  PROT_READ | PROT_WRITE, MAP_SHARED, shm_id, 0));
		  if (command == MAP_FAILED)
		  {
			  throw std::runtime_error("There was an error allocating memory: " + std::string(std::strerror(errno)));
		  }

		  bool flag;
		  std::string res = interp.interpret(command, flag);

		  munmap(command, sizeof(command));
		  close(shm_id);

		  auto response_queue = mq_open(query.response_queue_name, O_WRONLY);
		  if (response_queue == -1)
		  {
			  throw std::runtime_error(
				  "There was an error initializing message queue: " + std::string(std::strerror(errno)));
		  }

		  char response_msg[2000];
		  std::sprintf(response_msg, "%s", res.data());

		  mq_send(response_queue, response_msg, sizeof(response_msg), 0);
		  mq_close(response_queue);

		  if (flag)
		  {
			  break;
		  }
	  }
	});
	t.join();
}
