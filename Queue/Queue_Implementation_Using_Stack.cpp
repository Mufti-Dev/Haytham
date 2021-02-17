/*A simple queue implementation using std::stack*/
// Author: Mufti Md

#include <iostream>
#include <stack>

template <class T>
class Queue {
public:
    void push(T data) {
        _instack.push(data);
    }
    
    int front() {
        move();
        return _outStack.top();
    }

    void pop() {
        move();
        return _outStack.pop();
    }
private:
    void move() const {
        if (_outStack.empty()) {
            while (!_instack.empty()) {
                _outStack.push(_instack.top());
                _instack.pop();
            }
        }
    }
private:
    mutable std::stack<T> _instack;
    mutable std::stack<T> _outStack;
};

int main()
{
    Queue<int> qt;
    qt.push(100);
    qt.push(200);
    qt.push(300);

    std::cout << qt.front() << std::endl;
    qt.pop();
    std::cout << qt.front() << std::endl;
    return 0;
}