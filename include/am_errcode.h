

#ifndef __AM_ERRCODE_H__
#define __AM_ERRCODE_H__

enum AM_ERR
{
    ME_OK = 0,
    ME_WAIT,
    ME_ERROR,
    ME_CLOSED,
    ME_BUSY,
    ME_INTERNAL_ERROR,
    ME_NO_IMPL,
    ME_OS_ERROR,
    ME_IO_ERROR,
    ME_TIMEOUT,
    ME_NO_MEMORY,
    ME_TOO_MANY,

    ME_NOT_EXIST,
    ME_NOT_SUPPORT,
    ME_NO_INTERFACE,
    ME_BAD_STATE,
    ME_BAD_PARAM,
    ME_BAD_COMMAND,

    ME_UNKNOWN_FMT,
};

#endif

