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
class PicSource
{
public:
	PicSource(int i_csp,  int i_width, int i_height );
	~PicSource();
	static void delete_x264_picture(x264_picture_t* pic);
	static x264_picture_t* get_x264_picture(int i_csp,  int i_width, int i_height );
private:
	int csp,width,height;
	FrameQueue<x264_picture_t*> pPicIn;

private:
	//线程


};

#endif /* PICSOURCE_H_ */
