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

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	CircularBuffer<int, 4> buffer;

	Producer prod(buffer);
	Consumer con(buffer);

	pthread_t prod_id;
	pthread_t con_id;

	const int ret = pthread_create(&prod_id, NULL, Producer::run, &prod);
	printf("debug ---> thread return %d\n", ret);

	const int ret2 = pthread_create(&con_id, NULL, Consumer::run, &con);
	printf("debug ---> thread 2 return %d\n", ret2);

	pthread_join(prod_id, (void **) NULL);
	pthread_join(con_id, (void **) NULL);

	return 0;
}
