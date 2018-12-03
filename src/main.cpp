//============================================================================
// Name        : producer_consumer.cpp
// Author      : bert kong
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstdio>
#include "producer-consumer.hpp"

using namespace std;

class Message
{
public:
    Message(const int value=0)
        :m_value(value)
    {
    }

    Message(const Message &msg)
        :m_value(msg.m_value)
    {
    }

    ~Message()
    {
    }

    const Message &operator=(const Message &msg)
    {
        m_value = msg.m_value;

        return *this;
    }

    const int getValue() const
    {
        return m_value;
    }


private:
    mutable int m_value;
};


int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	CircularBuffer<Message, 4> buffer;

	Producer<Message> prod(buffer);
	Consumer<Message> con(buffer);

	pthread_t prod_id;
	pthread_t con_id;

	const int ret = pthread_create(&prod_id, NULL, Producer<Message>::run, &prod);
	printf("debug ---> thread return %d\n", ret);

	const int ret2 = pthread_create(&con_id, NULL, Consumer<Message>::run, &con);
	printf("debug ---> thread 2 return %d\n", ret2);

	pthread_join(prod_id, (void **) NULL);
	pthread_join(con_id, (void **) NULL);

	return 0;
}
