#pragma once
#include "GFXThrowables.h"
#include "HakuLog.h"
/*MACROS FOR SHINENGANS*/
#define EXCEPT_LAST_THROW() Throwables(__LINE__,__FILE__,__func__,GetLastError())


#if defined _DEBUG
#define EXCEPT_HR_THROW(Result) if(Result != S_OK){\
HAKU_LOG_ERR("ERROR! HR:", Result); throw Throwables(__LINE__,__FILE__,__func__,Result);}
#define GFX_EXCEPT_HR_THROW(Result) HAKU_LOG_ERR("ERROR! HR:", Result); throw GFXThrowables(__LINE__,__FILE__,__func__,Result);
#else
#define EXCEPT_HR_THROW(Result) if(Result != S_OK){\
throw Throwables(__LINE__,__FILE__,__func__,Result);}
#define GFX_EXCEPT_HR_THROW(Result) throw GFXThrowables(__LINE__,__FILE__,__func__,Result);
#endif
