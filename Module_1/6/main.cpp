/*
  Vlasov MM WEB-13

  6_1. Реализуйте стратегию выбора опорного элемента “медиана трёх”.
  Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу.

  Дано множество целых чисел из [0..10^9] размера n.
  Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры множества:
  10%  перцентиль
          медиана
  90%  перцентиль

  Требования: к дополнительной памяти: O(n).
  Среднее время работы: O(n)
  Должна быть отдельно выделенная функция partition.
  Рекурсия запрещена.
  Решение должно поддерживать передачу функции сравнения снаружи.


  Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении.
  Описание для случая прохода от начала массива к концу:
  Выбирается опорный элемент. Опорный элемент меняется с последним элементом массива.
  Во время работы Partition в начале массива содержатся элементы, не бОльшие опорного.
  Затем располагаются элементы, строго бОльшие опорного. В конце массива лежат нерассмотренные элементы.
  Последним элементом лежит опорный.
  Итератор (индекс) i указывает на начало группы элементов, строго бОльших опорного.
  Итератор j больше i, итератор j указывает на первый нерассмотренный элемент.
  Шаг алгоритма. Рассматривается элемент, на который указывает j. Если он больше опорного, то сдвигаем j.
  Если он не больше опорного, то меняем a[i] и a[j] местами, сдвигаем i и сдвигаем j.
  В конце работы алгоритма меняем опорный и элемент, на который указывает итератор i.
*/

#include <iostream>
#include "cassert"

// analog of std::less
template<class T>
class Less {
public:
    bool operator()( const T& l, const T& r ) { return l < r; }
};

template<class T, class Comparator = Less<T>>
int MedOf3(T *arr, const int& left, const int& right, Comparator cmp = Less<T>()) {
    int mid = (right - left + 1) / 2 + left;
    // if l > mid >= r or l < mid <= r
    if (!(cmp(arr[mid], arr[left]) ^ !cmp(arr[mid], arr[right]))) return mid;
    // if r <= l < mid or r >= l > mid
    if (!(cmp(arr[left], arr[mid]) ^ !cmp(arr[left], arr[right]))) return left;
    return right;
}


template <class T, class Comparator = Less<T>>
int Partition(T* arr, const int& left, const int& right, Comparator cmp = Less<T>()) {
    if (right - left < 1) {
        return left;
    }
    int pivot = MedOf3(arr, left, right);
    std::swap(arr[pivot], arr[right]);
    // i, j iterators from left to end
    // i points to pivot position after partitioning
    // it guarantees that all elements left from i are < arr[pivot]
    int i = left;
    for (int j = left; j < right; ++j) {
        if (cmp(arr[j], arr[right])) {
            std::swap(arr[j], arr[i++]);
        }
    }
    std::swap(arr[i], arr[right]);
    return i;
}

int Percentile(const int& p, const int& size) {
    return (int) ((double) p / 100 * (double) size);
}

template <class T, class Comparator = Less<T>>
T KOrderStat(T* arr, const int& size, const int& k, Comparator cmp = Less<T>()) {
    int left = 0;
    int right = size - 1;
    int pivot = Partition<T>(arr, left, right, cmp);
    while (pivot != k) {
        if (!cmp(k, pivot))
            left = pivot + 1;
        else
            right = pivot - 1;
        pivot = Partition<T>(arr, left, right, cmp);
    }
    return arr[k];
}

template <class T,  class Comparator = Less<T>>
void Task(T* arr, int size, Comparator cmp = Less<T>()) {
    int k1 = Percentile(10, size);
    int k2 = Percentile(50, size);
    int k3 = Percentile(90, size);

    std::cout << KOrderStat(arr, size, k1) << std::endl;
    std::cout << KOrderStat(arr, size, k2) << std::endl;
    std::cout << KOrderStat(arr, size, k3) << std::endl;
}

int main() {
    int n;
    std::cin >> n;
    int* arr = new int[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
        assert(arr[i] < 100000000);
    }
    Task(arr, n);
    delete[] arr;
    return 0;
}