#pragma once
#ifdef _DEBUG
#include "spdlog/sinks/wincolor_sink.h"
#include "spdlog/spdlog.h"
//to allocate a console window and initiate a intra-translation unit logger that post messages to stdhandle
#define HAKU_CONSOLE_INIT AllocConsole()
void Init_Logger();
void log_info(std::string msg);
void log_warn(std::string msg);
void log_crit(std::string msg);
void log_err(std::string msg);
//was macros type agnostic the whole time..?
//GENERAL THROW MACROS WHICH WOULD ALL BE STRIPPED TO NULL LINES
#define HAKU_LOG_INFO(msg) log_info(msg)
#define HAKU_LOG_WARN(msg) log_warn(msg)
#define HAKU_LOG_CRIT(msg) log_crit(msg)
#define HAKU_LOG_ERR(msg)  log_err(msg)
#define HAKU_LOG_INIT Init_Logger()
#endif
#ifndef _DEBUG
#define HAKU_CONSOLE_INIT 
#define HAKU_CONSOLE_SINK 
#define HAKU_CONSOLE_LOG  

#define HAKU_LOG_INFO(msg)
#define HAKU_LOG_WARN(msg)
#define HAKU_LOG_CRIT(msg)
#define HAKU_LOG_ERR(msg)
#define HAKU_LOG_INIT
#endif