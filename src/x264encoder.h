#ifndef  _X264ENCODER_H_
#define  _X264ENCODER_H_

#include <sys/time.h>
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include "x264.h"
#ifdef __cplusplus
}
#endif
#include "PicSource.h"

class x264encoder;
class PicSource;
class x264encoder
{
/****************************************************
* 使用我最常见的yuv420 格式编码
*
****************************************************/

public:
	//* 构造函数，初始化编码器参数结构,设置分辨率帧率等信息
	x264encoder( PicSource *ps);
	virtual ~x264encoder();
	//* 初始化编码器
	void init();
	//* 开始编码
	void Start();
	//* 停止编码
	void Stop();
private:
	//图片源
	PicSource *ps;
	bool bIsStart;

private:
	//* x264编码器的参数
	x264_param_t* pX264Param;
	//* x264编码器句柄
	x264_t* pX264Handle;
	
	//编码输出图像参数
	x264_picture_t *pPicOut;

	int framerate;
private:
	//编码线程，与线程锁
	AM_ERR Encoder();
	//静态函数
	static AM_ERR ThreadEncoder(void *p);
	//线程
	CamThread *G_pTimerThread;
	//
	CamCondition *cc;
	//锁
	CamMutex     *cm;

private:
	//编码之间的延时
	struct timeval tv_old,tv_new;
	inline void delay();
};




#endif
