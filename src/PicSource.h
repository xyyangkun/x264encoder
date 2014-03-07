/*
 * PicSource.h
 *
 *  Created on: 2014-3-7
 *      Author: yangkun
 */

#ifndef PICSOURCE_H_
#define PICSOURCE_H_
#include <queue>
class PicSource
{
public:
	enum PicType
	{
		YUV420 = 1,/*内存格式，不是平面格式，占 width*height*1.5*/
		YUV422 = 2,/*内存格式，不是平面格式，占 width*height*2*/
	};
	PicSource(int iWidth, int iHeight,PicType pType=YUV420);
	virtual ~PicSource();
private:
	PicType pType;
	int iWidth,iHeight;
	char *pPicBuf;
	unsigned int uPicBufSize;


};

#endif /* PICSOURCE_H_ */
