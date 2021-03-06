/*
 * producer-consumer.hpp
 *
 *  Created on: Nov 18, 2018
 *      Author: bert
 */

#ifndef PRODUCER_CONSUMER_HPP_
#define PRODUCER_CONSUMER_HPP_

#include <pthread.h>
#if 0
class iNotify
{
public:
	virtual void notify() = 0;
};

//template<class KLASS>
class Message
{
public:
	Message()
	  :id(-1), value(0),
	   m_received(false)
	{
		::pthread_mutex_init(&m_lock, NULL);
	}
	~Message()
	{
		::pthread_mutex_destroy(&m_lock);
	}

	void setId(const int id) const
	{
		m_id = id;
	}

	const int getId() const
	{
		return m_id;
	}

	const int getValue() const
	{
		return m_value;
	}

	void setValue(const int val) const
	{
		m_value = val;
	}

	const bool is_received() const
	{
		//m_sender->notify();
		if(::pthread_mutex_trylock(&m_lock)==0)
		{

		}
		const bool ret = m_received;

		m_received = false;
		return ret;
	}

private:
	mutable int m_id;
	mutable int m_value;
	mutable bool m_received;

	pthread_mutex_t m_lock;

	//KLASS *m_sender;
};
#endif


//----------------------------------------
//    Producer-Consumer Data
//----------------------------------------
template<typename T, int N>
class CircularBuffer
{
public:
    CircularBuffer()
        :m_size(N),
		 m_read_pos(0),
         m_write_pos(0)
    {
		::pthread_mutexattr_t *mutex_attr = nullptr;
		::pthread_mutex_init(&m_lock, mutex_attr);

#if defined(__WAIT_QUEUE__)
		::pthread_condattr_t *cond_attr = nullptr;
		::pthread_cond_init(&write_wait_queue, cond_attr);
		::pthread_cond_init(&read_wait_queue, cond_attr);
#endif
    }

    ~CircularBuffer()
    {
    }

    const bool isFull() const 
    {
        return (m_read_pos == (m_write_pos + 1)%m_size);
    }

    const bool isEmpty() const
    {
        return m_read_pos==m_write_pos;
    }

    const bool put(const int &value)
    {
        bool ret = false;

        if (::pthread_mutex_trylock(&m_lock)==0)
        {
            if (isFull())
            {
                ::pthread_mutex_unlock(&m_lock);
                return false;
            }
            
            m_buffer[m_write_pos] = value;
            m_write_pos = (m_write_pos + 1) % m_size;


            ret = true;
            ::pthread_mutex_unlock(&m_lock);
            
        }

        return ret;
    } 

    const bool get(int &value)
    {
        bool ret = false;

        if (::pthread_mutex_trylock(&m_lock)==0)
        {
            if (isEmpty())
            {
                ::pthread_mutex_unlock(&m_lock);
                return ret;
            }

            value = m_buffer[m_read_pos];
            m_read_pos = (m_read_pos + 1)%m_size;

            ret = true;

            ::pthread_mutex_unlock(&m_lock);
        }

        return ret;
    }


private:

#if defined(__WAIT_QUEUE__)
	pthread_cond_t write_wait_queue;
	pthread_cond_t read_wait_queue;
#endif

	const int m_size;
	T m_buffer[N];
	pthread_mutex_t m_lock;
	int m_read_pos;
	int m_write_pos;
};


//----------------------------------------
//    Producer Class
//----------------------------------------
class Producer
{
public:
    enum
    {
        NUMBER_OF_SLOTS = 4,
    };

public:
	static void *run(void * data);

public:
	Producer(CircularBuffer<int, NUMBER_OF_SLOTS> &buf)
		 :m_buffer(buf),
		  m_value(0)
	{
	}

	~Producer() {}


private:
	void write();
	//void lock();
	//void unlock();
	//void wait_on_write_queue();

	//void wakeup_read_queue();
	//const bool is_buffer_full() const;

private:
    CircularBuffer<int, NUMBER_OF_SLOTS> &m_buffer;

	int m_value;
};

//----------------------------------------
//    Consumer Class
//----------------------------------------
class Consumer
{
public:
    enum
    {
        NUMBER_OF_SLOTS = 4,
    };

public:
	static void *run(void * data);

public:
	Consumer(CircularBuffer<int, NUMBER_OF_SLOTS> &buf)
         :m_buffer(buf)
	{
	}

	~Consumer() {}

private:
	void read();
	//void wait_on_read_queue();
	//void wakeup_write_queue();

	//const bool is_buffer_empty() const;

private:
    CircularBuffer<int, NUMBER_OF_SLOTS> &m_buffer;
};

#endif /* PRODUCER_CONSUMER_HPP_ */
