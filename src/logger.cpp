#include <cstring>
#include <logger.hpp>

void Logger::init(std::string path)
{
	this->fs.open(path.data(), std::fstream::in | std::fstream::out | std::fstream::app);
	if (!this->fs.is_open() || !this->fs.good())
	{
		throw std::runtime_error(
			"There was an error initializing logging library: " + std::string(std::strerror(errno)));
	}
}

void Logger::log(Logger::Severity sv, std::string msg)
{
	std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::lock_guard<std::mutex> lock_guard(this->lock);
	fs << std::ctime(&time) << " " << get_sv_str(sv) << ": " << msg << std::endl;
}

std::string Logger::get_sv_str(Logger::Severity sv)
{
	return sv == 0
		   ? "MIN"
		   : sv == 1 ? "STANDARD" : "MAX";
}
