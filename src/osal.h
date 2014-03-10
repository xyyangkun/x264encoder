/*
 * osal.h
 *
 *  Created on: 2014-3-9
 *      Author: xy
 */

#ifndef OSAL_H_
#define OSAL_H_
// we need to include these 2 header files in this header file, or all users will have to include
// the header files to resolve pthread_mutex_t, etc.
#include "semaphore.h"
#include "pthread.h"

#include "am_errcode.h"
#include "am_types.h"
#include "am_debug.h"


#define MW_THREAD_ENTRY    *


class CamMutex;
class CAutoLock;
class CamSemaphore;
class CamCondition;
class CamEvent;
class CamThread;


//-----------------------------------------------------------------------
//
// CamMutex
//
//-----------------------------------------------------------------------
class CamMutex
{
    friend class CamCondition;

public:
    static CamMutex* Create();
    void Delete();

public:
    void Lock() { pthread_mutex_lock(&_hMutex); }
    void Unlock() { pthread_mutex_unlock(&_hMutex); }

private:
    CamMutex() {}
    AM_ERR Construct();
    ~CamMutex() { pthread_mutex_destroy(&_hMutex); }

private:
    pthread_mutex_t _hMutex;
};

//-----------------------------------------------------------------------
//
// CAutoLock
//
//-----------------------------------------------------------------------
class CAutoLock
{
public:
    CAutoLock(CamMutex *pMutex): _pMutex(pMutex) { _pMutex->Lock(); }
    ~CAutoLock() { _pMutex->Unlock(); }

private:
    CamMutex *_pMutex;
};

#define AUTO_LOCK(pMutex)       CAutoLock __lock__(pMutex)
#define _BEGIN_LOCK(pMutex)     pMutex->Lock()
#define _END_LOCK(pMutex)       pMutex->Unlock()


//-----------------------------------------------------------------------
//
// CamSemaphore
//
//-----------------------------------------------------------------------
class CamSemaphore
{
public:
    static CamSemaphore* Create(AM_UINT intCount);
    void Delete();

public:
    void Get() { sem_wait(&_hSemaphore); }
    void Release(AM_UINT releaseCount = 1) { sem_post(&_hSemaphore); }

private:
    CamSemaphore() {}
    AM_ERR Construct(AM_UINT intCount);
    ~CamSemaphore() { sem_destroy(&_hSemaphore); }

private:
    sem_t   _hSemaphore;
};

//-----------------------------------------------------------------------
//
// CamCondition
//
//-----------------------------------------------------------------------
class CamCondition
{
public:
    static CamCondition* Create();
    void Delete();

public:
    void Wait(CamMutex *pMutex)
    {
        int err = pthread_cond_wait(&_cond, &pMutex->_hMutex);
        if(err!=0)
        {
        	printf("err:%d",err);
        	throw(0);
        }
        AM_ASSERT(err == 0);
        err = err;
    }

    void Signal()
    {
        int err = pthread_cond_signal(&_cond);
        AM_ASSERT(err == 0);
        err = err;
    }

private:
    CamCondition()
    {
        int err = pthread_cond_init(&_cond, NULL);
        AM_ASSERT(err == 0);
        err = err;
    }
    ~CamCondition() {}

private:
    pthread_cond_t _cond;
};


//-----------------------------------------------------------------------
//
// CamEvent
//
//-----------------------------------------------------------------------
class CamEvent
{
public:
    static CamEvent* Create();
    void Delete();

public:
    AM_ERR Wait(int ms = -1)
    {
        sem_wait(&_hEvent);
        return ME_OK;
    }
    void Signal()
    {
        sem_trywait(&_hEvent);
        sem_post(&_hEvent);
    }

private:
    CamEvent() {}
    AM_ERR Construct();
    ~CamEvent() { sem_destroy(&_hEvent); }

private:
    sem_t   _hEvent;
};

//-----------------------------------------------------------------------
//
// CamThread
//
//-----------------------------------------------------------------------
typedef AM_ERR (*AM_THREAD_FUNC)(void *);
class CamThread
{
public:
    static CamThread* Create(const char *pName, AM_THREAD_FUNC entry, void *p);
	//added by ljy
    static CamThread* CreatePriority(const char *pName, AM_THREAD_FUNC entry, void *p);
    virtual void SetThreadPriority(int priority);

    void Delete();

public:
    enum
    {
        PRIO_HIGH,
        PRIO_NORMAL,
        PRIO_LOW,
    };
    static AM_ERR SetRTPriority(int priority);
    const char *Name() { return _pName; }

protected:
    CamThread(const char *pName);
	//modified by ljy
    virtual AM_ERR Construct(AM_THREAD_FUNC func, void *p);
    ~CamThread();

protected:
    static void MW_THREAD_ENTRY _Entry(void *p);
    void Entry();

protected:
    bool        _bThreadCreated;
    pthread_t   _hThread;
    const char *_pName;

    AM_THREAD_FUNC _func;
    void *_p;

	int _priority;
};

//added by ljy
class CamThreadPriority:public CamThread
{
public:
	CamThreadPriority(const char *pName);
	virtual AM_ERR Construct(AM_THREAD_FUNC func, void *p);
};
#endif /* OSAL_H_ */
