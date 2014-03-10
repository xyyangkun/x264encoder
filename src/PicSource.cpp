/*
 * PicSource.cpp
 *
 *  Created on: 2014-3-7
 *      Author: yangkun
 */

#include "PicSource.h"
#include <iostream>

PicSource::PicSource(int i_csp,  int i_width, int i_height)
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
