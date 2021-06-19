#include "HakuLog.h"
#ifdef _DEBUG
static std::shared_ptr<spdlog::sinks::wincolor_stderr_sink_st> WindowSink = nullptr;
static std::shared_ptr<spdlog::logger> DebugLogger = nullptr;

void Init_Logger()
{
	WindowSink = std::make_shared<spdlog::sinks::wincolor_stderr_sink_st>(spdlog::color_mode::automatic);
	DebugLogger = std::make_shared<spdlog::logger>("Haku", WindowSink);
	DebugLogger->set_pattern("[%n]%^%l:%o %v%$");
}


void log_out_info(const std::string msg) noexcept
{
	DebugLogger->info(msg);
}

void log_out_warn(const std::string msg) noexcept
{
	DebugLogger->warn(msg);
}

void log_out_crit(const std::string msg) noexcept
{
	DebugLogger->critical(msg);
}

void log_out_err(const std::string msg) noexcept
{
	DebugLogger->error(msg);
}




std::shared_ptr<spdlog::logger> getlog() noexcept
{
	return DebugLogger;
}
#endif 