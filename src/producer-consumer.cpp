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
void *Producer::run(void * data)
{
	Producer *prod = static_cast<Producer *>(data);

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

#if 0
const bool Producer::is_buffer_full() const
{
	printf("thread -----> %s\n", __func__);
	return (m_data.read_pos==m_data.write_pos);
}

void Producer::lock()
{
	::pthread_mutex_lock(&m_data.lock);
}

void Producer::unlock()
{
	::pthread_mutex_unlock(&m_data.lock);
}


void Producer::wakeup_read_queue()
{
	::pthread_cond_signal(&m_data.read_wait_queue);
}

void Producer::wait_on_write_queue()
{
	::pthread_cond_wait(&m_data.write_wait_queue,
                        &m_data.lock);
}
#endif

void Producer::write()
{
	if (m_buffer.put(m_value))
	{
		m_value += 1;
	}
}


//----------------------------------------------
//   Consumer Implementation
//----------------------------------------------
void *Consumer::run(void * data)
{
	Consumer *con = static_cast<Consumer *>(data);
    bool done = true;

	printf("thread -----> 3 %s\n", __func__);

	while(done)
	{
		con->read();
	}

	return nullptr;
}

void Consumer::read()
{
	int value;
	if (m_buffer.get(value))
	{
		::printf("debug ---> %d\n", value);
	}
}


#if 0
const bool Consumer::is_buffer_empty() const
{
	return (m_data.read_pos==m_data.write_pos+1);
}

void Consumer::lock()
{
	::pthread_mutex_lock(&m_data.lock);
}

void Consumer::unlock()
{
	::pthread_mutex_unlock(&m_data.lock);
}

void Consumer::wait_on_read_queue()
{
	::pthread_cond_wait(&m_data.read_wait_queue, &m_data.lock);
}

void Consumer::wakeup_write_queue()
{
	::pthread_cond_signal(&m_data.write_wait_queue);
}

#endif



