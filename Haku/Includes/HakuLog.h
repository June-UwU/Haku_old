#pragma once
#ifdef _DEBUG
#include "spdlog/sinks/wincolor_sink.h"
#include "spdlog/spdlog.h"
#include <cstdarg>
//to allocate a console window and initiate a intra-translation unit logger that post messages to stdhandle
#define HAKU_CONSOLE_INIT AllocConsole()
void Init_Logger();
template<typename Type , typename ...Types>
void _StringAssemble(std::string& ret,Type& arg1, Types&...arg2);
template <typename Type, typename ... Types>
void log_info(Type& arg1, Types& ...arg2);
void log_out_info(const std::string msg);
template <typename Type,typename ... Types>
void log_warn(Type& arg1, Types& ...arg2);
void log_out_warn(const std::string msg);
template<typename Type,typename...Types>
void log_crit(Type& arg1, Types...arg2);
void log_out_crit(const std::string msg);
template<typename Type,typename ...Types>
void log_err(Type& arg1, Types...arg2);
void log_out_err(const std::string msg);
//GENERAL THROW MACROS WHICH WOULD ALL BE STRIPPED TO NULL LINES
#define HAKU_LOG_INIT Init_Logger()
#define HAKU_LOG_INFO(...) log_info(__VA_ARGS__)
#define HAKU_LOG_WARN(...) log_warn(__VA_ARGS__)
#define HAKU_LOG_CRIT(...) log_crit(__VA_ARGS__)
#define HAKU_LOG_ERR(...) log_err(__VA_ARGS__)


template<typename Type, typename ...Types>
void _StringAssemble(std::string& ret, Type& arg1, Types& ...arg2)
{
	ret.append(arg1);
	ret.append(" ");
	if constexpr (sizeof...(arg2) > 0)
	{
		_StringAssemble(ret, arg2...);
	}
}

template<typename Type, typename ...Types>
void log_info(Type& arg1, Types & ...arg2)
{
	std::string msg(arg1);
	msg.append(" ");
	if constexpr (sizeof...(arg2) > 0)
	{
		_StringAssemble(msg, arg2...);
	}
	log_out_info(msg);
};

template<typename Type, typename ...Types>
void log_warn(Type& arg1, Types& ...arg2)
{
	std::string msg(arg1);
	msg.append(" ");
	if constexpr (sizeof...(arg2) > 0)
	{
		_StringAssemble(msg, arg2...);
	}
	log_out_warn(msg);
}

template<typename Type, typename ...Types>
void log_crit(Type& arg1, Types ...arg2)
{
	std::string msg(arg1);
	msg.append(" ");
	if constexpr (sizeof...(arg2))
	{
		_StringAssemble(msg, arg2...);
	}
	log_out_crit(msg);
}

template<typename Type, typename ...Types>
void log_err(Type& arg1, Types ...arg2)
{
	std::string msg(arg1);
	msg.append(" ");
	if constexpr (sizeof...(arg2))
	{
		_StringAssemble(msg, arg2...);
	}
	log_out_err(msg);
}

#endif
#ifndef _DEBUG
#define HAKU_CONSOLE_INIT 

#define HAKU_LOG_INFO(...)
#define HAKU_LOG_WARN(...)
#define HAKU_LOG_CRIT(...)
#define HAKU_LOG_ERR(...)
#define HAKU_LOG_INIT
#endif

