/*
 * producer-consumer.cpp
 *
 *  Created on: Nov 18, 2018
 *      Author: bert
 */

#include "producer-consumer.hpp"
#include <cstdio>

//--------------------------------------------
//  Producer Implementation
//--------------------------------------------
template<typename T>
void *Producer<T>::run(void * data)
{
	Producer<T> *prod = static_cast<Producer<T> *>(data);

	bool done=true;

	printf("thread -----> 1 %s\n", __func__);

	while(done)
	{
		prod->write();
		//prod->wakeup_read_queue();
		//prod->unlock();
	}

	return nullptr;
}

template<typename T>
void Producer<T>::write()
{
	T msg(m_value);
	
	if (m_buffer.put(msg))
	{
		m_value += 1;
	}
}


//----------------------------------------------
//   Consumer Implementation
//----------------------------------------------
template<typename T>
void *Consumer<T>::run(void * data)
{
	Consumer<T> *con = static_cast<Consumer<T> *>(data);
    bool done = true;

	printf("thread -----> 3 %s\n", __func__);

	while(done)
	{
		con->read();
	}

	return nullptr;
}

template<typename T>
void Consumer<T>::read()
{
	T msg;
	if (m_buffer.get(msg))
	{
		::printf("debug ---> %d\n", msg.getValue());
	}
}


