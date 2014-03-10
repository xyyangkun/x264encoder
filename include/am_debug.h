
#ifndef __DEBUG_H__
#define __DEBUG_H__

#define AM_DEBUG
//#undef AM_DEBUG


#ifdef AM_DEBUG

#include "stdio.h"
#include "assert.h"

#define AM_PRINTF(format, args...)      printf(">>> " format, ##args)
#define AM_ERROR(format, args...)       printf("!!! %s:%d " format, __FILE__, __LINE__, ##args)
#define AM_INFO(format, args...)        printf("::: " format, ##args)
#define AM_ASSERT(expr)                 assert(expr)
#define AM_PERROR(msg)			perror(msg);

#else

#define AM_PRINTF(format, args...)
#define AM_ERROR(format, args...)
#define AM_INFO(format, args...)
#define AM_ASSERT(expr)
#define AM_PERROR(msg)

#endif

#ifdef AM_LOG_EANBLE
#define AM_LOG(format, args...)         printf("*** " format, ##args)
#else
#define AM_LOG(format, args...)
#endif

#endif

