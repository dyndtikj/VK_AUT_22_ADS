/*
  Vlasov MM WEB-13

  Задача № 1.1
  Подсчитать кол-во единичных бит в входном числе , стоящих
  на четных позициях. Позиции битов нумеруются с 0.

  Во всех задачах необходимо использование битовых операций.
  Использование арифметических операций запрещено.
  Входное число лежит в диапазоне 0..2^32 -1 и вводится в десятичном виде

  Ввод/вывод отделены от решения.
  Не должно быть утечек памяти, за каждую утечку памяти - штраф “-1”.
 */

#include <iostream>

// time log(n)
// memory O(1)
uint CountSetEvenBits2(uint num) {
    size_t result = 0;
    // from 0 bit to last, while number > 0
    // after each iteration shift on 2 bit right and get the current 0 pos bit
    while(num){
        if ((num & 1) == 1) {
            result++;
        }
        num >>=2;
    }
    return result;
}

int main() {
    uint val = 0;
    std::cin >> val;
    std::cout << CountSetEvenBits2(val);
    return 0;
}
