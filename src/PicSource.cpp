/*
 * PicSource.cpp
 *
 *  Created on: 2014-3-7
 *      Author: yangkun
 */

#include "PicSource.h"
#include <iostream>

PicSource::PicSource(int iWidth, int iHeight, PicType pType)
{
	this->iWidth = iWidth;
	this->iHeight= iHeight;
	this->pType = pType;
	if(pType == YUV420)
	{
		uPicBufSize = iWidth*iHeight*3/2;
	}
	else if(pType == YUV422)
	{
		uPicBufSize = iWidth*iHeight*2;
	}
	else
	{
		throw 0;
	}
	pPicBuf = new char(uPicBufSize);

}

PicSource::~PicSource()
{
	delete pPicBuf;
	pPicBuf = NULL;
}

