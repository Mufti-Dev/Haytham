#include "QueueImplClass.h"
#include <iostream>
#include <cassert>

template<class T>
QueueImplClass<T>::QueueImplClass(int size) {
	capacity = size;
	arrQ = new T[size];
	assert(arrQ != nullptr);
	elementCount = 0;
	front = 0;
	back = -1;
}

template<class T>
bool QueueImplClass<T>::isEmpty() {
	return (elementCount == 0);
}

template<class T>
int QueueImplClass<T>::getSize() {
	return elementCount;
}

template<class T>
T QueueImplClass<T>::getFront() {
	if (isEmpty()) {
		std::cerr << "Empty Queue" << std::endl;
		exit(1);
	}
	return arrQ[front];
}

template<class T>
void QueueImplClass<T>::enqueue(T val) {
	if (elementCount == capacity) {
		std::cerr << "Queue Full" << std::endl;
		return;
	}

	if (back == capacity - 1) back = -1;
	arrQ[++back] = val;
	elementCount++;
}

template<class T>
T QueueImplClass<T>::dequeue() {
	if (isEmpty()) {
		std::cerr << "Queue Empty" << std::endl;
		exit(1);
	}

	T t = arrQ[front++];
	if (front == capacity) front = 0;
	elementCount--;
	return t;
}

template<class T>
void QueueImplClass<T>::showQueue() {
	int i = front;
	int count = 0;
	 while(count != elementCount) {
		std::cout << '\t' << arrQ[i % capacity];
		count++;
		i++;
	}
    std::cout << std::endl;
}

template class QueueImplClass<int>;