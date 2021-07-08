#pragma once
#ifdef _DEBUG
#include <stdlib.h>
#include "spdlog/sinks/wincolor_sink.h"
#include "spdlog/spdlog.h"
#include <cstdarg>
//to allocate a console window and initiate a intra-translation unit logger that post messages to stdhandle
inline std::string Normalize(const std::string& msg) noexcept { return msg; }
inline std::string Normalize(const char* msg) noexcept { return std::string(msg); }
inline std::string Normalize(wchar_t* msg) noexcept
{
	char _Data[100]{};
	wcstombs(_Data, msg, 100);
	return std::string(_Data);
}
template<typename Type>
inline std::string Normalize(const Type msg) noexcept { return std::to_string(msg); }
#define HAKU_CONSOLE_INIT AllocConsole()
//init and reassignment is required static initialization silently fails
void Init_Logger();
template<typename Type , typename ...Types>
void _StringAssemble(std::string& ret,Type& arg1, Types&...arg2) noexcept;
template <typename Type, typename ... Types>
void log_info(Type& arg1, Types& ...arg2)noexcept;
void log_out_info(const std::string msg)noexcept;
template <typename Type,typename ... Types>
void log_warn(Type& arg1, Types& ...arg2)noexcept;
void log_out_warn(const std::string msg)noexcept;
template<typename Type,typename...Types>
void log_crit(Type& arg1, Types...arg2)noexcept;
void log_out_crit(const std::string msg)noexcept;
template<typename Type,typename ...Types>
void log_err(Type& arg1, Types...arg2)noexcept;
void log_out_err(const std::string msg)noexcept;


//GENERAL THROW MACROS WHICH WOULD ALL BE STRIPPED TO NULL LINES
#define HAKU_LOG_INIT Init_Logger()
#define HAKU_LOG_INFO(...) log_info(__VA_ARGS__)
#define HAKU_LOG_WARN(...) log_warn(__VA_ARGS__)
#define HAKU_LOG_CRIT(...) log_crit(__VA_ARGS__)
#define HAKU_LOG_ERR(...) log_err(__VA_ARGS__)


template<typename Type, typename ...Types>
void _StringAssemble(std::string& ret, Type& arg1, Types& ...arg2) noexcept
{
	ret.append(Normalize(arg1) + " ");
	if constexpr (sizeof...(arg2) > 0)
	{
		_StringAssemble(ret, arg2...);
	}
}

template<typename Type, typename ...Types>
void log_info(Type& arg1, Types & ...arg2) noexcept
{
	std::string msg(Normalize(arg1) + " ");
	if constexpr (sizeof...(arg2))
	{
		_StringAssemble(msg, arg2...);
	}
	log_out_info(msg);
};

template<typename Type, typename ...Types>
void log_warn(Type& arg1, Types& ...arg2) noexcept
{
	std::string msg(Normalize(arg1) + " ");
	if constexpr (sizeof...(arg2))
	{
		_StringAssemble(msg, arg2...);
	}
	log_out_warn(msg);
}

template<typename Type, typename ...Types>
void log_crit(Type& arg1, Types ...arg2) noexcept
{
	std::string msg(Normalize(arg1) + " ");
	if constexpr (sizeof...(arg2))
	{
		_StringAssemble(msg, arg2...);
	}
	log_out_crit(msg);
}

template<typename Type, typename ...Types>
void log_err(Type& arg1, Types ...arg2) noexcept
{
	std::string msg(Normalize(arg1) + " ");
	//msg.append(" ");
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

