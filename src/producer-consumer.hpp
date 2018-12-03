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

    const bool put(const T &obj)
    {
        bool ret = false;

        if (::pthread_mutex_trylock(&m_lock)==0)
        {
            if (isFull())
            {
                ::pthread_mutex_unlock(&m_lock);
                return false;
            }
            
            m_buffer[m_write_pos] = obj;
            m_write_pos = (m_write_pos + 1) % m_size;


            ret = true;
            ::pthread_mutex_unlock(&m_lock);
            
        }

        return ret;
    } 

    const bool get(T &obj)
    {
        bool ret = false;

        if (::pthread_mutex_trylock(&m_lock)==0)
        {
            if (isEmpty())
            {
                ::pthread_mutex_unlock(&m_lock);
                return ret;
            }

            obj = m_buffer[m_read_pos];
            m_read_pos = (m_read_pos + 1)%m_size;

            ret = true;

            ::pthread_mutex_unlock(&m_lock);
        }

        return ret;
    }


private:
	const int m_size;
	T m_buffer[N];
	pthread_mutex_t m_lock;
	int m_read_pos;
	int m_write_pos;
};



//----------------------------------------
//    Producer Class
//----------------------------------------
template<typename T>
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

private:
    CircularBuffer<int, NUMBER_OF_SLOTS> &m_buffer;

    int m_value
};

//----------------------------------------
//    Consumer Class
//----------------------------------------
template<typename T>
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

private:
    CircularBuffer<int, NUMBER_OF_SLOTS> &m_buffer;
};

#endif /* PRODUCER_CONSUMER_HPP_ */
