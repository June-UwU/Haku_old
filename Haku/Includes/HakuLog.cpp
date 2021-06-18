#include "HakuLog.h"
#ifdef _DEBUG
static std::shared_ptr<spdlog::sinks::wincolor_stderr_sink_st> WindowSink = nullptr;
static std::shared_ptr<spdlog::logger> DebugLogger = nullptr;

void Init_Logger()
{
	WindowSink = std::make_shared<spdlog::sinks::wincolor_stderr_sink_st>(spdlog::color_mode::automatic);
	DebugLogger = std::make_shared<spdlog::logger>("Haku", WindowSink);
}


void log_out_info(const std::string msg)
{
	DebugLogger->info(msg);
}

void log_out_warn(const std::string msg)
{
	DebugLogger->warn(msg);
}

void log_out_crit(const std::string msg)
{
	DebugLogger->critical(msg);
}

void log_out_err(const std::string msg)
{
	DebugLogger->error(msg);
}

std::string Normalize(const std::string& norm)
{
	return norm;
}

std::string Normalize(const char* norm)
{
	return std::string(norm);
}



std::shared_ptr<spdlog::logger> getlog() noexcept
{
	return DebugLogger;
}
#endif 