/*
 * FrameQueue.h
 *
 *  Created on: 2014-3-9
 *      Author: xy
 */

#ifndef FRAMEQUEUE_H_
#define FRAMEQUEUE_H_

#include <pthread.h>
#include <queue>

using std::queue;

template <typename T>
class FrameQueue
{
public:
	FrameQueue();
	void clear();
	void push(T);

	unsigned int size(){return frame.size();};
	T pop();
	virtual ~FrameQueue();
private:

	queue<T> frame;
	pthread_mutex_t counter_mutex;
};

template <typename T>
FrameQueue<T>::FrameQueue()
{
	 if(pthread_mutex_init(&counter_mutex,NULL)!=0)
		 throw(0);
}

template <typename T>
FrameQueue<T>::~FrameQueue()
{
	clear();
	pthread_mutex_destroy(&counter_mutex);
}

template <typename T>
void FrameQueue<T>::push(T bs)
{
	pthread_mutex_lock(&counter_mutex);
	frame.push(bs);
	pthread_mutex_unlock(&counter_mutex);
}



template <typename T>
T FrameQueue<T>::pop()
{
	pthread_mutex_lock(&counter_mutex);
	T bs = frame.front();
	frame.pop();
	pthread_mutex_unlock(&counter_mutex);
	return bs;
}

template <typename T>
void FrameQueue<T>::clear()
{
	while(frame.size()>0)
	{
		delete pop();
	};
}


#endif /* FRAMEQUEUE_H_ */
