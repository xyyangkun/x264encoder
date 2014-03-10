

#ifndef __AM_TYPES_H__
#define __AM_TYPES_H__

#include "types.h"

// general data types

typedef int AM_INT;
typedef unsigned int AM_UINT;

typedef unsigned char AM_U8;
typedef unsigned short AM_U16;
typedef unsigned long AM_U32;

typedef signed char AM_S8;
typedef signed short AM_S16;
typedef signed long AM_S32;

typedef signed long long AM_S64;
typedef unsigned long long AM_U64;

typedef unsigned char AM_CHAR;
typedef int AM_BOOL;

typedef AM_S64 AM_FILE_OFFSET_T;

#define AM_TRUE     1
#define AM_FALSE    0



// file

typedef AM_U64             AM_FILE_SIZE;
#define AM_FILE_SIZE_MAX    -1ULL

enum AM_SEEK_TYPE
{
    AM_SEEK_SET,
    AM_SEEK_CUR,
    AM_SEEK_END
};



// mw specific

typedef AM_U16  AM_PORT_ID;     // message port ID
typedef AM_U16  AM_MSG_ID;      // message ID



// message define - message system

struct AM_MSG
{
    AM_PORT_ID  portID;
    AM_MSG_ID   msgID;
    AM_UINT     syncCounter;
    int         p0;
    void        *p1;
    void        *p2;
};

#endif

