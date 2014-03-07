#include <cassert>
#include <string>
#include <iostream>

#include "x264encoder.h"
x264encoder::x264encoder(unsigned int width, unsigned int height)
{
	pX264Param = new x264_param_t;
	if(!pX264Param)
	{
		throw(0);
	}
	//配置参数
	//使用默认参数
	x264_param_default(pX264Param);
	//取空缓冲区继续使用不死锁的保证
	pX264Param->i_threads = X264_SYNC_LOOKAHEAD_AUTO;
	pX264Param->i_width = width;
	pX264Param->i_height = height;
	pX264Param->i_frame_total = 0; //编码总帧数，不知道用０
	pX264Param->i_keyint_max = 10;
	//* bitstream parameters
	pX264Param->i_bframe = 5;
	pX264Param->b_open_gop = 0;
	pX264Param->i_bframe_pyramid = 0;
	pX264Param->i_bframe_adaptive = X264_B_ADAPT_TRELLIS;

	//* log
	pX264Param->i_log_level = X264_LOG_DEBUG;
	//* Rate control Parameters
	pX264Param->rc.i_bitrate = 10* 1024 ; //* 码率(比特率，单位kbps)
	//* muxing parameters
	pX264Param->i_fps_den = 1; 	//* 帧率分母
	pX264Param->i_fps_num = 25;	//* 帧率分子
	pX264Param->i_timebase_den = pX264Param->i_fps_num;
	pX264Param->i_timebase_num = pX264Param->i_fps_den;

	//* 设置Profile.使用MainProfile
	x264_param_apply_profile(pX264Param, x264_profile_names[1]);
}
x264encoder::~x264encoder()
{
	//* 清除图像区域
	x264_picture_clean(pPicIn);
	x264_picture_clean(pPicOut);
	//* 关闭编码器句柄
	x264_encoder_close(pX264Handle);
	pX264Handle = NULL;
	
	delete pPicIn;
	pPicIn = NULL;
	
	delete pPicOut;
	pPicOut = NULL;
	
	delete pX264Param;
	pX264Param = NULL;
}
void x264encoder::init()
{
	//* 打开编码器句柄，通过x264_encoder_parameters得到设置给X264
	//* 的参数. 通x264_encoder_reconfig更新x264的参数
	pX264Handle = x264_encoder_open(pX264Param);
	if(!pX264Handle)
	{
		std::cout << "Init x264 encoder error!!" << std::endl;
	}

	//* Nal 这东西要在这设置吗？



	//* 获取允许缓存的最大帧数
	int iMaxFrames = \
		x264_encoder_maximum_delayed_frames(pX264Handle);
	std::cout << "delayed frames: " << iMaxFrames << std::endl;

	//*　编码需要的参数
	pPicIn = new x264_picture_t;
	pPicOut= new x264_picture_t;	
	
	x264_picture_init(pPicOut);
	x264_picture_alloc(pPicIn, X264_CSP_I420, pX264Param->i_width,\
				pX264Param->i_height);
	pPicIn->img.i_csp = X264_CSP_I420;
	pPicIn->img.i_plane = 3;

}

int x264encoder::add(unsigned char *framebuf, unsigned int bufsize)
{
	
	return 0;
}
