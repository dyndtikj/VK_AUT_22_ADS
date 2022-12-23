/*
 Vlasov MM WEB-13

 3_1. Реализовать очередь с динамическим зацикленным буфером.
 Требования: Очередь должна быть реализована в виде класса.
 Во всех задачах из следующего списка следует написать структуру данных, обрабатывающую команды push*
 и pop*.

 Формат входных данных.
 В первой строке количество команд n. n ≤ 1000000.
 Каждая команда задаётся как 2 целых числа: a b.
 a = 1 - push front
 a = 2 - pop front
 a = 3 - push back
 a = 4 - pop back
 Команды добавления элемента 1 и 3 заданы с неотрицательным параметром b.
 Для очереди используются команды 2 и 3. Для дека используются все четыре команды.
 Если дана команда pop*, то число b - ожидаемое значение. Если команда pop вызвана для пустой
 3_1. Реализовать очередь с динамическим зацикленным буфером.
 Требования: Очередь должна быть реализована в виде класса.
 структуры данных, то ожидается “-1”.
 Формат выходных данных.
 Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание
 не оправдалось, то напечатать NO.
 */

#include <iostream>
#include "cassert"

// Implemeted methods 2 , 3 - push back, pop front
// Not template class, type - int
// queue data is under protected specificator to enable access for children classes
class Queue {
protected:
    int *_data;
    // can be less then real quantity of elements
    // because head can move and this field shows only buffer size
    // to grow if needed
    int _capacity;
    // the real size of queue
    int _size;
    // indexes
    int _head;
    int _tail;
    // coefficient for expanding buffer
    const int kGrow = 2;
protected:
    void grow() {
        int newCapacity = (_capacity > 0) ? _capacity * kGrow : 1;
        int *newData = new int[newCapacity];
        // copies element from old buffer to new
        // if tail was < head, after this operation head = 0, tail = _capacity - 1
        for (int i = 0; i < _capacity; i++) {
            newData[i] = _data[(i+_head)%_capacity];
        }
        // update fields
        // size is same
        _head = 0;
        _tail = (_capacity > 0) ? _capacity - 1 : 0;
        _capacity = newCapacity;
        // delete previous buffer and make data point to new buffer
        delete[] _data;
        _data = newData;
    }

public:
    Queue() : _data(nullptr), _capacity(0), _size(0), _head(0), _tail(0) {
        //grow();
    }

    ~Queue() {
        delete []_data;
    }

    int size() {
        return _size;
    }

    int capacity(){
        return _capacity;
    }

    int popFront() {
        if (_size) {
            // if are elements in queue
            int value = _data[_head];
            _head = (_head+1)%_capacity;
            _size--;
            return value;
        }
        return -1;
    }

    void pushBack(int value) {
        if (_capacity == _size){
            grow();
        }
        _tail = (_tail+1)%_capacity;
        _data[_tail] = value;
        _size++;
    }

    int getFront(){
        return _data[_head];
    }
    int getBack() {
        return _data[_tail];
    }
};

int Task(const int& num){
    Queue queue;
    int command = 0, value = 0;
    for (int i = 0; i < num; ++i) {
        std::cin >> command >> value;
        if (command == 2) {
            int front = queue.popFront();
            if (front != value) {
                std::cout << "NO";
                return 0;
            }
            continue;
        }
        if (command == 3){
            queue.pushBack(value);
            continue;
        }
        std::cout << "commamd number not acceptable for this queue";
        return 1;
    }
    std::cout <<  "YES" << std::endl;
    return 0;
}

int main() {
    // quantity of commands
    int com_num = 0;
    std::cin >> com_num;
    assert(com_num > 0 && com_num <= 1000000);
    return Task(com_num);
}