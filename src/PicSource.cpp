/*
 * PicSource.cpp
 *
 *  Created on: 2014-3-7
 *      Author: yangkun
 */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include "PicSource.h"

PicSource::PicSource(int i_csp,  int i_width, int i_height)\
:bIsStart(false)
{
	int csp = i_csp & X264_CSP_MASK;
	if (csp <= X264_CSP_NONE || csp >= X264_CSP_MAX || csp == X264_CSP_V210)
		throw(-1);
	this->csp = csp;
	this->width=i_width;
	this->height=i_height;

}

PicSource::~PicSource()
{
	cm->Delete();
	cc->Delete();
	G_pTimerThread->Delete();
}
//static
void PicSource::delete_x264_picture(x264_picture_t* pic)
{
	if(pic==NULL)
		throw 0;
	//* 清除图像区域
	x264_picture_clean(pic);

	delete pic;
	pic = NULL;


}
//static 获取x264格式的源图片
x264_picture_t* PicSource::get_x264_picture(int i_csp,  int i_width, int i_height )
{
	x264_picture_t* pPicIn = new x264_picture_t;
	x264_picture_alloc(pPicIn, i_csp, i_width, i_height);
	pPicIn->img.i_csp = i_csp;

	// FixMe
	pPicIn->img.i_plane = 3;
	return pPicIn;
}
void PicSource::Start()
{
	cm = CamMutex::Create();
	if(NULL == cm)
		throw(0);
	cc = CamCondition::Create();
	bIsStart=true;
	if ((G_pTimerThread = CamThread::Create("create pic", ThreadCreatePic, this)) == NULL)
		throw(0);

}
void PicSource::Stop()
{
	cc->Signal();
	bIsStart=false;
}
AM_ERR  PicSource::ThreadCreatePic(void *p)
{
	PicSource *ps=(PicSource *)p;
	return ps->CreatePic();
}
AM_ERR PicSource::CreatePic()
{
	static int uiComponent=0;
	while(1)
	{
		//大于0,等待
		if(pPicIn.size()>1)
		{
			AUTO_LOCK(cm); //加锁等待
			cc->Wait(cm);
		}
		if(!bIsStart)
			break;
		//获取x264图片内存
		x264_picture_t* x264_pic = get_x264_picture(csp, width, height);
		int iDataLen = width * height;
		uint8_t* data = new uint8_t[iDataLen];
#if 1
		uiComponent = rand()%255;
#else
		if(uiComponent++>255)
			uiComponent=0;
#endif
		//printf("color: %#X\n",uiComponent);
		//* 构建需要编码的源数据(YUV420色彩格式)
		memset(data, uiComponent, iDataLen);

		memcpy(x264_pic->img.plane[0], data, iDataLen);
		memcpy(x264_pic->img.plane[1], data, iDataLen / 4);
		memcpy(x264_pic->img.plane[2], data, iDataLen / 4);
		delete data;
		//把图片放到队列中
		pPicIn.push(x264_pic);
	}
	return ME_OK;
	printf("debug picSource CREATE4444444444444\n");
}
x264_picture_t*  PicSource::GetPic()
{
	//如果队列里面没有数据了，发送信号
	while(pPicIn.size()<=1)
	{
		//printf("no enough data!!!\n");
		cc->Signal();
	}
	return pPicIn.pop();
}
