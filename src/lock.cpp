#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include "lock.hpp"

void Lock::create_lock(std::string pid_file)
{
	struct flock pid_lck = { F_WRLCK, SEEK_SET, 0, 0, 0 };

	int pid_fd = open(pid_file.data(), O_CREAT | O_WRONLY, 0640);
	if (pid_fd == -1)
	{
		throw std::runtime_error("There was an error opening PID file: " + std::string(std::strerror(errno)));
	}

	if (fcntl(pid_fd, F_SETLK, &pid_lck) == -1)
	{
		close(pid_fd);
		throw std::runtime_error("There was an error setting PID file lock: " + std::string(std::strerror(errno)));
	}

	char pid_lock_buf[11];
	std::sprintf(pid_lock_buf, "%d\n", getpid());
	write(pid_fd, pid_lock_buf, std::strlen(pid_lock_buf) + 1);
}

bool Lock::is_locked(std::string pid_file)
{
	struct flock pid_lck = { F_WRLCK, SEEK_SET, 0, 0, 0 };

	int pid_fd = open(pid_file.data(), O_RDONLY, 0640);
	if (pid_fd == -1)
	{
		return false;
	}

	if (fcntl(pid_fd, F_GETLK, &pid_lck) == -1)
	{
		close(pid_fd);
		throw std::runtime_error("There was an error reading PID file lock status: " + std::string(std::strerror(errno)));
	}

	close(pid_fd);
	return pid_lck.l_type != F_UNLCK;
}