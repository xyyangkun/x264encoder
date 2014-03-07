#ifdef __cplusplus
extern "C" {
#endif

#include "x264.h"

#ifdef __cplusplus
}
#endif


int main(int argc, char** argv)  
{  
     int iResult = 0;  
     x264_t* pX264Handle   = NULL;  
     x264_param_t* pX264Param = new x264_param_t;  
     assert(pX264Param);  
     //* 配置参数  
     //* 使用默认参数，在这里因为我的是实时网络传输，所以我使用了zerolatency的选项，使用这个选项之后就不会有delayed_frames，如果你使用的不是这样的话，还需要在编码完成之后得到缓存的编码帧  
     x264_param_default_preset(pX264Param, "veryfast", "zerolatency");  
     //* cpuFlags  
     pX264Param->i_threads  = X264_SYNC_LOOKAHEAD_AUTO;//* 取空缓冲区继续使用不死锁的保证.  
     //* 视频选项  
     pX264Param->i_width   = 352; //* 要编码的图像宽度.  
     pX264Param->i_height  = 288; //* 要编码的图像高度  
     pX264Param->i_frame_total = 0; //* 编码总帧数.不知道用0.  
     pX264Param->i_keyint_max = 10;   
     //* 流参数  
     pX264Param->i_bframe  = 5;  
     pX264Param->b_open_gop  = 0;  
     pX264Param->i_bframe_pyramid = 0;  
     pX264Param->i_bframe_adaptive = X264_B_ADAPT_TRELLIS;  
     //* Log参数，不需要打印编码信息时直接注释掉就行  
     pX264Param->i_log_level  = X264_LOG_DEBUG;  
     //* 速率控制参数  
     pX264Param->rc.i_bitrate = 1024 * 10;//* 码率(比特率,单位Kbps)  
     //* muxing parameters  
     pX264Param->i_fps_den  = 1; //* 帧率分母  
     pX264Param->i_fps_num  = 10;//* 帧率分子  
     pX264Param->i_timebase_den = pX264Param->i_fps_num;  
     pX264Param->i_timebase_num = pX264Param->i_fps_den;  
     //* 设置Profile.使用Baseline profile  
     x264_param_apply_profile(pX264Param, x264_profile_names[0]);  
  
     //* 编码需要的辅助变量  
     iNal = 0;  
     pNals = NULL;  
     x264_picture_t* pPicIn = new x264_picture_t;  
     x264_picture_t* pPicOut = new x264_picture_t;  
     x264_picture_init(pPicOut);  
     x264_picture_alloc(pPicIn, X264_CSP_I420, pX264Param->i_width, pX264Param->i_height);  
     pPicIn->img.i_csp = X264_CSP_I420;  
     pPicIn->img.i_plane = 3;  
  
     //* 打开编码器句柄,通过x264_encoder_parameters得到设置给X264  
     //* 的参数.通过x264_encoder_reconfig更新X264的参数  
     pX264Handle = x264_encoder_open(pX264Param);  
     assert(pX264Handle);  
  
     //* 创建文件,用于存储编码数据  
     FILE* pFile = fopen("test.264", "wb");  
     assert(pFile);  
     //设置y4m文件参数  
    y4m_input_t *y4m_hnd = (y4m_input_t*)malloc(sizeof(y4m_input_t));  
    //打开y4m文件  
    iResult = open_file_y4m("benchmark.y4m",(hnd_t**)&y4m_hnd,pX264Param);  
    if(iResult < 0 )  
    {  
        printf("Failed to open file!\n");  
        return 0;  
    }  
    //得到文件总得帧数  
    int nFrames = ::get_frame_total_y4m((hnd_t*)y4m_hnd);  
    //开始编码  
    for(int i = 0; i < nFrames ; i++ )  
    {  
        //读取一帧  
        read_frame_y4m(pPicIn,(hnd_t*)y4m_hnd,i);  
        if( i ==0 )  
            pPicIn->i_pts = i;  
        else  
            pPicIn->i_pts = i - 1;  
          
        //编码  
        int frame_size = x264_encoder_encode(pX264Handle,&pNals,&iNal,pPicIn,pPicOut);  
  
        if(frame_size >0)  
        {  
              
            for (int i = 0; i < iNal; ++i)  
            {//将编码数据写入文件.  
                fwrite(pNals[i].p_payload, 1, pNals[i].i_payload, pFile);  
            }  
        }  
    }  
  
     //* 清除图像区域  
     x264_picture_clean(pPicIn);  
     //* 关闭编码器句柄  
     x264_encoder_close(pX264Handle);  
     pX264Handle = NULL;  
     delete pPicIn ;  
     pPicIn = NULL;  
     delete pPicOut;  
     pPicOut = NULL;  
     delete pX264Param;  
     pX264Param = NULL;  
     return 0;  
}  
