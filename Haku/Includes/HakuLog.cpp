#include "HakuLog.h"
#ifdef _DEBUG
static std::shared_ptr<spdlog::sinks::wincolor_stderr_sink_st> WindowSink = nullptr;
static std::shared_ptr<spdlog::logger> DebugLogger = nullptr;

void Init_Logger()
{
	WindowSink = std::make_shared<spdlog::sinks::wincolor_stderr_sink_st>(spdlog::color_mode::automatic);
	DebugLogger = std::make_shared<spdlog::logger>("LOG", WindowSink);
}

void log_info(std::string msg)
{
	DebugLogger->log(spdlog::level::level_enum::info, msg);
}

void log_warn(std::string msg)
{
	DebugLogger->log(spdlog::level::level_enum::warn, msg);
}

void log_crit(std::string msg)
{
	DebugLogger->log(spdlog::level::level_enum::critical, msg);
}

void log_err(std::string msg)
{
	DebugLogger->log(spdlog::level::level_enum::err, msg);
}

#endif 