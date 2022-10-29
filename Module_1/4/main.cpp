/*
 Vlasov MM - WEB-13
 В операционной системе Technux есть планировщик процессов.
 Каждый процесс характеризуется:
 приоритетом P
 временем, которое он уже отработал t
 временем, которое необходимо для завершения работы процесса T

 Планировщик процессов выбирает процесс с минимальным значением P * (t + 1),
 выполняет его время P и кладет обратно в очередь процессов.

 Если выполняется условие t >= T, то процесс считается завершенным и удаляется
 из очереди. Требуется посчитать кол-во переключений процессора. Формат входных
 данных:  Сначала вводится кол-во процессов. После этого процессы в формате P T
 Формат выходных данных: Кол-во переключений процессора.
*/

#include <cassert>
#include <cstring>
#include <iostream>

class Process {
protected:
    int priority;  // P
    int tWorked;   // t
    int tToWork;   // T
public:
    Process();
    Process(int, int, int);
    void Update();
    bool IsFinished();
    bool operator<(const Process& r) const;
};

Process::Process(int P, int t, int T) : priority(P), tWorked(t), tToWork(T) {}

Process::Process() : priority(0), tWorked(0), tToWork(0) {}
void Process::Update() { tWorked += priority; }
bool Process::IsFinished() { return tWorked >= tToWork; }
// operator < , needed for comparator
bool Process::operator<(const Process& r) const {
    return priority * (tWorked + 1) < r.priority * (r.tWorked + 1);
}

// std::less does the same
template <class T>
class Less {
public:
    bool operator()(const T& l, const T& r) { return l < r; }
};

template <class T, class Comparator = Less<T>>
class Heap {
public:
    Heap(T* array, int sizeArr, Comparator cmp = Less<T>());
    Heap& operator=(const Heap& other) = delete;
    ~Heap();
    T ExtractMin();
    bool IsEmpty();
    T& GetMin();
    void SiftDown(int idx);

private:
    int size_;
    T* arr_;
    Comparator cmp_;
};

template <class T, class Compare>
Heap<T, Compare>::Heap(T* array, int sizeArr, Compare comp) {
    size_ = sizeArr;
    arr_ = new T[size_];
    memcpy(arr_, array, sizeof(T) * size_);
    cmp_ = comp;
    for (int i = size_ / 2; i >= 0; --i) {
        SiftDown(i);
    }
}

template <class T, class Compare>
void Heap<T, Compare>::SiftDown(int idx) {
    int left = idx * 2 + 1;
    int right = idx * 2 + 2;
    int min = idx;

    // if element exists and comparator returned 1
    if (left < size_ && cmp_(arr_[left], arr_[min])) {
        min = left;
    }
    if (right < size_ && cmp_(arr_[right], arr_[min])) {
        min = right;
    }
    if (min != idx) {
        std::swap(arr_[min], arr_[idx]);
        SiftDown(min);
    }
}

template <class T, class Compare>
T Heap<T, Compare>::ExtractMin() {
    // get first element (minimal) and recover heap ( seep last element )
    assert(!IsEmpty());
    T val = arr_[0];
    arr_[0] = arr_[--size_];
    if (!IsEmpty()) {
        SiftDown(0);
    }
    return val;
}

template <class T, class Compare>
bool Heap<T, Compare>::IsEmpty() {
    return size_ == 0;
}

template <class T, class Compare>
T& Heap<T, Compare>::GetMin() {
    assert(!IsEmpty());
    return arr_[0];
}

template <class T, class Compare>
Heap<T, Compare>::~Heap() {
    delete[] arr_;
}

int RunProcs(Heap<Process>& procs) {
    int num = 0;
    // perform processes till heap is not empty
    while (!procs.IsEmpty()) {
        // perform process
        Process& last = procs.GetMin();
        last.Update();
        // if finished - exctract else sift to correct position
        if (last.IsFinished()) {
            procs.ExtractMin();
        } else {
            procs.SiftDown(0);
        }
        num++;
    }
    return num;
}

int main() {
    // input
    int n;
    std::cin >> n;
    Process* processes = new Process[n];
    for (int i = 0; i < n; ++i) {
        int P = 0, T = 0, t = 0;
        std::cin >> P >> T;
        processes[i] = Process(P, t, T);
    }
    Heap<Process> ps_heap(processes, n);
    // logic
    int num = RunProcs(ps_heap);
    // output
    std::cout << num << std::endl;
    delete[] processes;
    return 0;
}