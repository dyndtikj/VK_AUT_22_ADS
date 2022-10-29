/*
  Vlasov MM WEB-13

  Требование для всех вариантов Задачи 5

  Во всех задачах данного раздела необходимо реализовать и использовать сортировку слиянием в виде
  шаблонной функции.
  Решение должно поддерживать передачу функции сравнения снаружи.
  Общее время работы алгоритма O(n log n).
  5_1. Реклама.
  В супермаркете решили оптимизировать показ рекламы. Известно расписание прихода и ухода покупателей
  (два целых числа). Каждому покупателю необходимо показать минимум 2 рекламы.

  Рекламу можно транслировать только в целочисленные моменты времени. Покупатель может видеть
  рекламу от момента прихода до момента ухода из магазина.
  В каждый момент времени может показываться только одна реклама.
  Считается, что реклама показывается мгновенно. Если реклама показывается в момент ухода
  или прихода, то считается, что посетитель успел её посмотреть. Требуется определить минимальное
  число показов рекламы.
*/

#include <iostream>
#include <cassert>
#include <cstring>

// analog of std::less
template <class T>
class Less {
public:
    bool operator()(const T& l, const T& r) {
        return l < r;
    }
};

template <class T, class Comparator = Less<T>>
void Merge(T *arr1, T *arr2, int arr1Len, int arr2Len, Comparator cmp = Less<T>()) {
    // allocate new buffer for merged
    int len = arr1Len + arr2Len;
    T *result = new T[len];
    // pointers for arr1 and arr2
    int ptr1 = 0;
    int ptr2 = 0;
    // pointers for merged array
    int resPtr = 0;
    // if only one pointer goes to end - merge almost completed
    while (ptr1 < arr1Len && ptr2 < arr2Len) {
        if (cmp(arr2[ptr2], arr1[ptr1])) {
            result[resPtr] = arr2[ptr2];
            ++ptr2;
        } else {
            result[resPtr] = arr1[ptr1];
            ++ptr1;
        }
        ++resPtr;
    }
    // append last elements of sorted array 1 (after all elements of 2 were added)
    while (ptr1 < arr1Len) {
        result[resPtr] = arr1[ptr1];
        ++ptr1;
        ++resPtr;
    }
    // append last elements of sorted array 1 (after all elements of 2 were added)
    while (ptr2 < arr2Len) {
        result[resPtr] = arr2[ptr2];
        ++ptr2;
        ++resPtr;
    }
    memcpy(arr1, result, sizeof(T) * (len));
    delete[] result;
}

template <class T, class Comparator = Less<T>>
void MergeSort(T *arr, int arrSize, Comparator cmp = Less<T>()) {
    // base of recursion
    if (arrSize <= 1) {
        return;
    }
    int mid = arrSize / 2;
    MergeSort(arr, mid, cmp);
    MergeSort(arr + mid, arrSize - mid, cmp);
    Merge(arr, arr + mid, mid, arrSize - mid, cmp);
}

struct Client {
    int in = 0;
    int out = 0;
    int ads_num = 0;

    Client(): in(0), out(0), ads_num(0){}

    Client(const int& t_in, const int& t_out, const int& ads):
            in(t_in), out(t_out), ads_num(ads){}

    bool operator <(const Client &r) const{
        if ((out == r.out && in > r.in) || out < r.out)
            return true;
        return false;
    }
};

// adsMin - ads minimum quantity per human
int CountMinAds(Client *clients,const int& num_clients, const int& adsMin = 2){
    MergeSort(clients, num_clients);
    int ads_num = 0;
    int idx = 0;
    while (idx != num_clients) {
        if (clients[idx].ads_num >= 2) {
            ++idx;
            continue;
        }
        for (int i = idx + 1; i < num_clients && clients[i].in <= clients[idx].out ; ++i) {
            for (int j = 0; j < adsMin - clients[idx].ads_num; ++j) {
                if (clients[i].in <= clients[idx].out - j) {
                    ++clients[i].ads_num;
                }
            }
        }
        ads_num += (clients[idx].ads_num == 0 ? adsMin : adsMin-clients[idx].ads_num);
        clients[idx].ads_num = adsMin;
        idx++;
    }
    return ads_num;
}

int main() {
    int num_clients = 0;
    std::cin >> num_clients;
    assert(num_clients > 0);
    Client *clients = new Client[num_clients];
    for (int i = 0; i < num_clients; ++i) {
        std::cin >> clients[i].in >> clients[i].out;
    }
    std::cout << CountMinAds(clients, num_clients);
    delete[] clients;
    return 0;
}