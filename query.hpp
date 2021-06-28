#ifndef _QUERY_HPP_
#define _QUERY_HPP_

#include <iostream>

struct query_t
{
	char response_queue_name[256];
	char shm_name[256];
	size_t shm_size;
};

#endif //_QUERY_HPP_
