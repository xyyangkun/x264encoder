/*
 * PicSource.h
 *
 *  Created on: 2014-3-7
 *      Author: yangkun
 */

#ifndef PICSOURCE_H_
#define PICSOURCE_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include "x264.h"
#ifdef __cplusplus
}
#endif

#include "FrameQueue.h"
#include "osal.h"
#include "x264encoder.h"
class x264encoder;
class PicSource;
class PicSource
{
	friend class x264encoder;

public:
	PicSource(int i_csp,  int i_width, int i_height );
	~PicSource();
	//删除一个x264图片的内存
	static void delete_x264_picture(x264_picture_t* pic);
	//初始化一个x264图片的内存
	static x264_picture_t* get_x264_picture(int i_csp,  int i_width, int i_height );
	//开始创造（接收）图片
	void Start();
	//停止创造（接收）图片
	void Stop();
	//从队列里面获取图片
	x264_picture_t* GetPic();
private:
	int csp,width,height;
	//图片队列
	FrameQueue<x264_picture_t*> pPicIn;
	bool bIsStart;
private:
	AM_ERR CreatePic();
	//静态函数
	static AM_ERR ThreadCreatePic(void *p);
	//线程
	CamThread *G_pTimerThread;
	//
	CamCondition *cc;
	//锁
	CamMutex     *cm;
};

#endif /* PICSOURCE_H_ */
