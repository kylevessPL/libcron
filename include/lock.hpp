#ifndef _LOCK_HPP_
#define _LOCK_HPP_

#include <string>

class Lock
{
 public:
	static void create_lock(std::string pid_file);
	static bool is_locked(std::string pid_file);
};

#endif //_LOCK_HPP_
