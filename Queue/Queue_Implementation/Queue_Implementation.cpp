// Queue_Implementation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "QueueImplClass.h"

int main()
{
    QueueImplClass<int> queue(5);
    queue.enqueue(2);
    queue.enqueue(4);
    queue.enqueue(6);
    queue.enqueue(8);
    queue.enqueue(10);
    queue.showQueue();
    std::cout << "Adding 6th element to a queue with capacity equal to 5: " << std::endl;
    queue.enqueue(12);

    queue.showQueue();
    queue.dequeue();
    queue.dequeue();
    queue.showQueue();
    std::cout << "The size of queue is: " << queue.getSize() << std::endl;
    queue.enqueue(14);
    queue.enqueue(16);
    queue.showQueue();
    std::cout << "The size of queue is: " << queue.getSize() << std::endl;

    return 0;
}