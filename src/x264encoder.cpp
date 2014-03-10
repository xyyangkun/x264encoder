#include <cassert>
#include <string>
#include <iostream>

#include "x264encoder.h"

x264encoder::x264encoder(PicSource *ps)
:framerate(10)
{
	this->ps = ps;
	tv_old.tv_sec  = 0;
	tv_old.tv_usec = 0;
	tv_new.tv_sec  = 0;
	tv_new.tv_usec = 0;
	pX264Param = new x264_param_t;
	if(!pX264Param)
	{
		throw(0);
	}
	//配置参数
	//使用默认参数
	//x264_param_default(pX264Param);
	//使用实时的编码
	x264_param_default_preset(pX264Param, "veryfast", "zerolatency");
	//取空缓冲区继续使用不死锁的保证
	pX264Param->i_threads = X264_SYNC_LOOKAHEAD_AUTO;
	pX264Param->i_width = ps->width;
	pX264Param->i_height = ps->height;
	pX264Param->i_frame_total = 0; //编码总帧数，不知道用０
	pX264Param->i_keyint_max = 10;
	//pX264Param->i_keyint_min = 10;


	//实时编码不管b帧
#if 1
	//* bitstream parameters
	pX264Param->i_bframe = 0;//5;
	pX264Param->b_open_gop = 0;
	pX264Param->i_bframe_pyramid = 0;
	pX264Param->i_bframe_adaptive = X264_B_ADAPT_TRELLIS;
#endif
	//* log 如果想看调试信息可以打开
	//pX264Param->i_log_level = X264_LOG_DEBUG;
	//* Rate control Parameters
	pX264Param->rc.i_bitrate = 10* 1024 ; //* 码率(比特率，单位kbps)
	//* muxing parameters
	pX264Param->i_fps_den = 1; 	//* 帧率分母
	pX264Param->i_fps_num = framerate;	//* 帧率分子
	pX264Param->i_timebase_den = pX264Param->i_fps_num;
	pX264Param->i_timebase_num = pX264Param->i_fps_den;

	//* 设置Profile.使用MainProfile
	x264_param_apply_profile(pX264Param, x264_profile_names[1]);
}

x264encoder::~x264encoder()
{
	//清除线程相关内容
	cm->Delete();
	cc->Delete();
	G_pTimerThread->Delete();

	//* 清除图像区域
	x264_picture_clean(pPicOut);
	//* 关闭编码器句柄
	x264_encoder_close(pX264Handle);
	pX264Handle = NULL;
	
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
	pPicOut= new x264_picture_t;	
	
	x264_picture_init(pPicOut);


}

void x264encoder::Start()
{
	ps->Start();

	cm = CamMutex::Create();
	if(NULL == cm)
		throw(0);
	cc = CamCondition::Create();
	bIsStart=true;
	if ((G_pTimerThread = CamThread::Create("create pic", ThreadEncoder, this)) == NULL)
		throw(0);
}
void x264encoder::Stop()
{
	bIsStart = false;
	ps->Stop();
}
AM_ERR x264encoder::ThreadEncoder(void *p)
{
	x264encoder *xe=(x264encoder *)p;
	return xe->Encoder();
}
AM_ERR x264encoder::Encoder()
{
	FILE* pFile = fopen("agnt.264", "wb");
	assert(pFile);
	int iNal = 0;
	x264_nal_t* pNals = NULL;
	while(bIsStart)
	{
		delay();
		//从队列中获取图片
		x264_picture_t* pic_in = ps->GetPic();
		int iResult = 0;
		iResult = x264_encoder_encode(pX264Handle, &pNals, &iNal, pic_in, pPicOut);
		if (0 == iResult)
		{
			//std::cout << "编码成功,但被缓存了." << std::endl;
		}
		else if (iResult < 0)
		{
			std::cout << "编码出错" << std::endl;
		}
		else if (iResult > 0)
		{
			//std::cout << "得到编码数据" << std::endl;
		}
		PicSource::delete_x264_picture(pic_in);
		pic_in = NULL;
		//int iFrames = x264_encoder_delayed_frames(pX264Handle);
		//std::cout << "当前编码器中缓存数据:" << iFrames << "帧\n";

		//* 将编码数据写入文件.
		for (int i = 0; i < iNal; ++i)
		{
			//printf("%#X\t%#X\n",pNals[i].p_payload[3],pNals[i].p_payload[4]);
			fwrite(pNals[i].p_payload, 1, pNals[i].i_payload, pFile);
		}
	}
	return ME_OK;
}

void x264encoder::delay()
{
	const int delaytime = 1000000/framerate;
	gettimeofday(&tv_new, NULL);
	int tmp = (tv_new.tv_sec - tv_old.tv_sec)*1000000 + (tv_new.tv_usec - tv_old.tv_usec);
	if( tmp < delaytime)
	{
		usleep(delaytime - tmp);
		printf("delay:\t\t%d\n",delaytime-tmp);
	}
	else
		printf("use time:\t%d\n",tmp);
	gettimeofday(&tv_old, NULL);
	int tmp2 = (tv_old.tv_sec - tv_new.tv_sec)*1000000 + (tv_old.tv_usec - tv_new.tv_usec);
	printf("real time:\t%d  %d,\n",tmp2, tmp2-(delaytime - tmp));
	gettimeofday(&tv_new, NULL);
	tv_old = tv_new;
}
