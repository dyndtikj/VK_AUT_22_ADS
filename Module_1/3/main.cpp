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

// time O(n), when calling grow()
// time O(1), without grow
// memory O(n)

#include <iostream>
#include <cassert>

// Implemeted methods 2 , 3 - push back, pop front
// Not template class, type - int
// queue data is under protected specificator to enable access for children classes
class Queue {
protected:
    int     *data_;
    // can be less then real quantity of elements
    // because head can move and this field shows only buffer size
    // to grow if needed
    int capacity_;
    // the real size of queue
    int size_;
    // indexes
    int head_;
    int tail_;
    // coefficient for expanding buffer
    const int k_grow = 2;
protected:
    void grow() {
        int newCapacity = (capacity_ > 0) ? capacity_ * k_grow : 1;
        int *newData = new int[newCapacity];
        // copies element from old buffer to new
        // if tail was < head, after this operation head = 0, tail = capacity_ - 1
        // (reorganization of array)
        for (int i = 0; i < capacity_; i++) {
            newData[i] = data_[(i+head_)%capacity_];
        }
        // update fields
        // size is same
        head_ = 0;
        tail_ = (capacity_ > 0) ? capacity_ - 1 : 0;
        capacity_ = newCapacity;
        // delete previous buffer and make data point to new buffer
        delete[] data_;
        data_ = newData;
    }

public:
    Queue() : data_(nullptr), capacity_(0), size_(0), head_(0), tail_(0) {
        //grow();
    }

    ~Queue() {
        delete []data_;
    }

    int Size() {
        return size_;
    }

    int Capacity(){
        return capacity_;
    }

    int PopFront() {
        if (size_) {
            // if are elements in queue
            int value = data_[head_];
            head_ = (head_+1)%capacity_;
            size_--;
            return value;
        }
        return -1;
    }

    void PushBack(int value) {
        if (capacity_ == size_){
            grow();
        }
        tail_ = (tail_+1)%capacity_;
        data_[tail_] = value;
        size_++;
    }

    int GetFront(){
        return data_[head_];
    }
    int GetBack() {
        return data_[tail_];
    }
};

int Task(int num){
    Queue queue;
    int command_num = 0, command_val = 0;
    for (int i = 0; i < num; ++i) {
        std::cin >> command_num >> command_val;
        if (command_num == 2) {
            int front = queue.PopFront();
            if (front != command_val) {
                std::cout << "NO";
                return 0;
            }
            continue;
        }
        if (command_num == 3){
            queue.PushBack(command_val);
            continue;
        }
        std::cout << "commamd number not acceptable for this queue";
        return 1;
    }
    std::cout <<  "YES" << std::endl;
}

int main() {
    int num = 0;
    std::cin >> num;
    assert(num > 0 && num <= 1000000);
    return Task(num);
}