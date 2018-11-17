//============================================================================
// Name        : template_tests.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <pthread.h>

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

class Producer
{
public:
	static void *run(void * unused)
	{
		Producer prod;
		Message msg;
		bool done=false;
		printf("thread 1 %s\n", __func__);

		msg.setId(2);
		msg.setValue(10);

		prod.write(msg);
		while(done)
		{
			if (!msg.is_received())
			{
				pthread_yield();
			}

			msg.setId(3);
			msg.setValue(11);
		}
	}
public:
	Producer()
	{
	}
	~Producer() {}

	void write(const Message &msg);


private:
};

class Consumer
{
public:
	static void *run(void *notused)
	{
		Consumer prod;
		printf("thread 2 %s\n", __func__);
	}
public:
	Consumer()
	{
	}
	~Consumer() {}

private:
};



int main() {

	pthread_t prod;
	pthread_t con;

	const int ret_stat = pthread_create(&prod, NULL, Producer::run, NULL);
	const int ret_stat2 = pthread_create(&con, NULL, Consumer::run, NULL);

	printf("thread 1 %d\n", ret_stat);
	printf("thread 2 %d\n", ret_stat2);


	return 0;
}
