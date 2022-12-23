/*
 Vlasov MM WEB-13

 2_3. Даны два массива неповторяющихся целых чисел, упорядоченные по возрастанию.
 A[0..n-1] и B[0..m-1]. n >> m. Найдите их пересечение.

 Требования: Время работы: O(m * log k), где k - позиция элемента B[m-1] в массиве A..
 В процессе поиска очередного элемента B[i] в массиве A пользуйтесь результатом поиска элемента B[i-1].

 Внимание! В этой задаче для каждого B[i] сначала нужно определить диапазон для бинарного поиска
 размером порядка k с помощью экспоненциального поиска, а потом уже в нем делать бинарный поиск.

 n, k ≤ 10000.
*/

#include <iostream>
#include <stdexcept>
template<class T>
class Vector
{
// queue data is under protected specificator to enable access for children classes
protected:
    T*  arr_;
    int size_;
    int capacity_;
public:
    Vector();
    ~Vector();

    void Append(T data);
    void Print();
    T    Pop(int index);
    T&   operator[](int index) const;
    int  Capacity() const;
    int  Size() const;
};

// ---------------------------------------------------------

int expSearch(int sizeA, const int A[], int value) {
    if (A[0] == value)
        return 0;

    int i = 1;
    while (i < sizeA && A[i] <= value)
        i *= 2;
    return i > sizeA ? sizeA : i;
}

Vector<int> arrIntersection(int sizeA,  int sizeB, const int A[], const int B[]) {
    Vector<int> result;
    // current left pos - after every search is moving
    // to use previous b[i] search result and search from
    // new left position of A
    int left = 0;
    // try to find every element of B
    for (int i = 0; i < sizeB; ++i) {
        // left index changes after binary search
        // resonably search from last left position with new size, and from new begin of array
        int right = left + expSearch(sizeA-left, &A[left], B[i]);
        //  binary search
        while (right - left > 1) {
            int mid = left + (right - left) / 2;
            if (A[mid] > B[i]) {
                right = mid;
            } else {
                left = mid;
            }
        }
        // checking BS result, add to list
        if (left < sizeA && A[left] == B[i]) {
            result.Append(B[i]);
        }
    }
    return result;
}

int main() {
    int sizeA = 0;
    int sizeB = 0;
    std::cin >> sizeA >> sizeB;
    int *A = new int[sizeA], *B = new int[sizeB];
    for (int i = 0; i < sizeA; ++i)
        std::cin >> A[i];
    for (int i = 0; i < sizeB; ++i)
        std::cin >> B[i];
    Vector<int> intersect = arrIntersection(sizeA, sizeB, A , B);
    intersect.Print();

    delete[] A;
    delete[] B;
    return 0;
}

template <class T>
Vector<T>::Vector()
        : arr_(nullptr), size_(0), capacity_(0)
{
}

template <class T>
Vector<T>::~Vector() {
    delete[] arr_;
}

template <class T>
void Vector<T>::Append(T data) {
    if(size_ == capacity_){
        if (!capacity_)
            capacity_++;
        else
            capacity_ *= 2;
        T *b = new T[capacity_];
        for(int i=0; i < size_; i++)
            b[i] = arr_[i];
        delete[] arr_;
        arr_ = b;
    }
    arr_[size_++] = data;
}

template <class T>
T Vector<T>::Pop(int index) {
    T val;
    if(index >= size_ || index < 0) {
        throw std::out_of_range("non valid index");
    }
    size_--;
    val = arr_[index];
    for(int i=index; i<size_; i++)
        arr_[i] = arr_[i+1];
    return val;
}

template <class T>
void Vector<T>::Print(){
    for(int i=0; i<size_; i++)
        std::cout << arr_[i] << " ";
    std::cout << std::endl;
}

template <class T>
T& Vector<T>::operator[](int index) const {
    if (index >= Size() || index < 0){
        throw std::out_of_range("non valid index");
    }
    return arr_[index];
}

template <class T>
int Vector<T>::Capacity() const {
    return capacity_;
}

template <class T>
int Vector<T>::Size() const {
    return size_;
}