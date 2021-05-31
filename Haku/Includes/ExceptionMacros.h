#pragma once


/*MACROS FOR SHINENGANS*/
#define EXCEPT_LAST_THROW() Throwables(__LINE__,__FILE__,__func__,GetLastError())
#define EXCEPT_HR_THROW(Result) if(Result != S_OK){throw Throwables(__LINE__,__FILE__,__func__,Result);}
