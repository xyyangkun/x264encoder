/*
 * osal.cpp
 *
 *  Created on: 2014-3-9
 *      Author: xy
 */

#include "osal.h"


//-----------------------------------------------------------------------
//
// CamMutex
//
//-----------------------------------------------------------------------
CamMutex* CamMutex::Create()
{
    CamMutex *result = new CamMutex;
    if (result != NULL && result->Construct() != ME_OK)
    {
        delete result;
        result = NULL;
    }
    return result;
}

void CamMutex::Delete()
{
    delete this;
}

AM_ERR CamMutex::Construct()
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&_hMutex, &attr);
    return ME_OK;
}

//-----------------------------------------------------------------------
//
// CamSemaphore
//
//-----------------------------------------------------------------------
CamSemaphore* CamSemaphore::Create(AM_UINT initCount)
{
    CamSemaphore *result = new CamSemaphore;
    if (result != NULL && result->Construct(initCount) != ME_OK)
    {
        delete result;
        result = NULL;
    }
    return result;
}

AM_ERR CamSemaphore::Construct(AM_UINT initCount)
{
    sem_init(&_hSemaphore, 0, initCount);
    return ME_OK;
}

void CamSemaphore::Delete()
{
    delete this;
}

//-----------------------------------------------------------------------
//
// CamCondition
//
//-----------------------------------------------------------------------
CamCondition* CamCondition::Create()
{
    return new CamCondition();
}

void CamCondition::Delete()
{
    delete this;
}

//-----------------------------------------------------------------------
//
// CamEvent
//
//-----------------------------------------------------------------------
CamEvent* CamEvent::Create()
{
    CamEvent* result = new CamEvent;
    if (result != NULL && result->Construct() != ME_OK)
    {
        delete result;
        result = NULL;
    }
    return result;
}

AM_ERR CamEvent::Construct()
{
    sem_init(&_hEvent, 0, 0);
    return ME_OK;
}

void CamEvent::Delete()
{
    delete this;
}

//-----------------------------------------------------------------------
//
// CamThread
//
//-----------------------------------------------------------------------
CamThread* CamThread::Create(const char *pName, AM_THREAD_FUNC entry, void *p)
{
	//modified by ljy
    	CamThread *result = NULL;
#if 0
	if( memcmp(pName,"AudioCapture",sizeof("AudioCapture")-1) == 0)
	{
		//printf("AAAAAAAAAAAAA %s\n", pName);
		//printf("%s+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n",pName);
		result = new CamThreadPriority(pName);

		result->SetThreadPriority(40);//0~99
	}
	if( memcmp(pName,"AAC ENCODER",sizeof("AAC ENCODER")-1) == 0)
	{
		//printf("AAAAAAAAAAAAA %s\n", pName);
		result = new CamThreadPriority(pName);

		result->SetThreadPriority(41);//0~99
	}
	else if (memcmp(pName,"VideoEncoder",sizeof("VideoEncoder")-1) == 0)
	{
		//printf("AAAAAAAAAAAAA %s\n", pName);
		result = new CamThreadPriority(pName);

		result->SetThreadPriority(42);//0~99
	}
	else
#endif
	{
		result = new CamThread(pName);
	}

    if (result != NULL && result->Construct(entry, p) != ME_OK)
    {
        delete result;
        result = NULL;
    }
    return result;
}
//added by ljy
CamThread* CamThread::CreatePriority(const char *pName, AM_THREAD_FUNC entry, void *p)
{
    CamThread *result = new CamThreadPriority(pName);
    if (result != NULL && result->Construct(entry, p) != ME_OK)
    {
        delete result;
        result = NULL;
    }
    return result;
}
void CamThread::SetThreadPriority(int priority)
{
	_priority=priority;
}

CamThread::CamThread(const char *pName):
    _bThreadCreated(false),
    _pName(pName),
	_priority(40)
{
}

void CamThread::Delete()
{
    delete this;
}

AM_ERR CamThread::Construct(AM_THREAD_FUNC func, void *p)
{
    _func = func;
    _p = p;
    pthread_create(&_hThread, NULL, _Entry, (void*)this);
    // todo - check return value
    _bThreadCreated = true;
    return ME_OK;
}

CamThread::~CamThread()
{
    if (_bThreadCreated)
    {
        pthread_join(_hThread, NULL);
    }
}

void MW_THREAD_ENTRY CamThread::_Entry(void *p)
{
    CamThread *pthis = (CamThread *)p;
    pthis->_func(pthis->_p);
    AM_PRINTF("thread %s exits\n", pthis->_pName);
    return NULL;
}

AM_ERR CamThread::SetRTPriority(int priority)
{
    struct sched_param param;
    param.sched_priority = 99;
    if (sched_setscheduler(0, SCHED_FIFO, &param) < 0)
        AM_PERROR("sched_setscheduler");
    AM_LOG("+++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    return ME_OK;
}

//added by ljy
CamThreadPriority::CamThreadPriority(const char *pName):
	CamThread(pName)
{
	//printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++CamThreadPriority::CamThreadPriority\n");
}
AM_ERR CamThreadPriority::Construct(AM_THREAD_FUNC func, void *p)
{
	//printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++CamThreadPriority::Construct\n");
	struct sched_param   param;
    	pthread_attr_t pthread_attr;
	pthread_attr_init(&pthread_attr);
  	param.sched_priority = _priority;/*sched_get_priority_max(SCHED_RR);*/
  	pthread_attr_setinheritsched(&pthread_attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&pthread_attr, SCHED_RR);
   	pthread_attr_setschedparam(&pthread_attr, &param);

    _func = func;
    _p = p;
    pthread_create(&_hThread, &pthread_attr, _Entry, (void*)this);
    // todo - check return value
    _bThreadCreated = true;
    return ME_OK;
}
