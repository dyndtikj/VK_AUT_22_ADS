/*
   Vlasov MM WEB-13

   7_3. Binary MSD для long long.
   Дан массив неотрицательных целых 64-разрядных чисел.
   Количество чисел не больше 10^6. Отсортировать массив методом MSD по битам (бинарный QuickSort).

 */
#include <iostream>
#include "cassert"

bool GetBit(const uint64_t& num, const size_t& pos) {
    return num & ((uint64_t)1 << pos);
}

// MSD
// For each bit position (from high to low pos)
// Sorts elemets only by current bit
// If not last bit - sorts two parts
void MSDSort(uint64_t *arr, size_t left, size_t right,
             const size_t& bit_pos) {

    size_t i = left;
    size_t j = right;

    if (j <= i || bit_pos < 0) { return; }

    while (i < j) {
        while (!GetBit(arr[i], bit_pos) && (i < j)) i++;
        while (GetBit(arr[j], bit_pos) && (i < j)) j--;
        // swap
        if (i != j) {
            uint64_t tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        }
    }
    // this if for situation if all bits on bit pos were 0
    // because now i=j=right and we must sort to right (not right -1)
    if (!GetBit(arr[right], bit_pos)) j++;
    if (j) MSDSort(arr, left, j - 1, bit_pos - 1);
    MSDSort(arr, j, right, bit_pos - 1);
}

void Task(uint64_t* arr, const size_t& size){
    // start sorting from 63_rd bit , because use uint64_t type
    MSDSort(arr, 0, size-1, 63);
    for (size_t i = 0; i < size; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
    return;
}

int main() {
    size_t size = 0;
    std::cin >> size;
    assert(size <= 1000000);

    uint64_t *arr = new uint64_t[size];
    for (size_t i = 0; i < size; ++i) {
        std::cin >> arr[i];
    }
    Task(arr, size);
    delete[] arr;
    return 0;
}