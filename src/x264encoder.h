#ifndef  _X264ENCODER_H_
#define _X264ENCODER_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include "x264.h"
#ifdef __cplusplus
}
#endif
class x264encoder
{
/****************************************************
* 使用我最常见的yuv420 格式编码
*
****************************************************/

public:
	//* 构造函数，初始化编码器参数结构,设置分辨率帧率等信息
	x264encoder(unsigned int width,unsigned int height);
	virtual ~x264encoder();
	//* 初始化编码器
	void init();
	//* 添加编码帧
	int add(unsigned char *framebuf, unsigned int bufsize);
private:
	//* x264编码器的参数
	x264_param_t* pX264Param;
	//* x264编码器句柄
	x264_t* pX264Handle;
	
	//编码输入输出图像参数
	x264_picture_t *pPicIn,*pPicOut;
};




#endif
