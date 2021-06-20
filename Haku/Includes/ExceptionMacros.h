#pragma once
#include "HakuLog.h"
/*MACROS FOR SHINENGANS*/
#define EXCEPT_LAST_THROW() Throwables(__LINE__,__FILE__,__func__,GetLastError())

#define EXCEPT_HR_THROW(Result) if(Result != S_OK){\
HAKU_LOG_ERR("ERROR! HR:", std::to_string(Result)); throw Throwables(__LINE__,__FILE__,__func__,Result);}

