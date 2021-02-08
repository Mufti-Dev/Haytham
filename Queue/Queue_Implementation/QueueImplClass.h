#pragma once
template<class T>
class QueueImplClass
{
private:
	int front;
	int back;
	int capacity;
	int elementCount;
	T* arrQ;

public:
	QueueImplClass(int size);
	bool isEmpty();
	T getFront();
	void enqueue(T val);
	T dequeue();
	int getSize();
	void showQueue();
};

