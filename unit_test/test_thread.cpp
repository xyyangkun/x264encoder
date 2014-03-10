/*
 * test_thread.cpp
 *
 *  Created on: 2014-3-9
 *      Author: xy
 */

#include "osal.h"
#include <stdio.h>
#include <unistd.h>
bool G_bExitTimerThread=false;
CamCondition *cc;
CamMutex     *cm;
AM_ERR TimerThread(void *p)
{
    while (!G_bExitTimerThread)
    {
    	AUTO_LOCK(cm); //加锁等待
    	cc->Wait(cm);
        printf("haha\n");

    }

    return ME_OK;
}

int main()
{
	CamThread *G_pTimerThread;
	cm = CamMutex::Create();
	if(NULL == cm)
		printf("debug0!\n");
	cc = CamCondition::Create();

	printf("debug!\n");
	if ((G_pTimerThread = CamThread::Create("timer", TimerThread, NULL)) == NULL)
		return -1;
	sleep(1);
	cc->Signal();

	sleep(5);
	cc->Signal();

	G_bExitTimerThread = 1;
	G_pTimerThread->Delete();

	cm->Delete();
	cc->Delete();
}
