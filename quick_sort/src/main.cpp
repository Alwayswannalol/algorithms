#include <iostream>
#include <vector>

#include "QuickSort.h"
#include "Array.h"

int main()
{
    std::vector<int> arr = {9, 3, 7, 4, 1, 8, 2, 5, 6};

    Array<int> my_arr{};
    for (int i = 0; i < arr.size(); i++) {
        my_arr.insert(arr[i]);
    }

    std::cout << "Исходный массив (vector): ";
    for (int num : arr)
        std::cout << num << " ";
    std::cout << std::endl;

    sort(arr.data(), arr.data() + arr.size(), [](int a, int b)
         { return a < b; });

    std::cout << "Отсортированный массив (vector): ";
    for (int num : arr)
        std::cout << num << " ";
    std::cout << std::endl;

    std::cout << "Исходный массив (Array): ";
    for (int num : my_arr)
        std::cout << num << " ";
    std::cout << std::endl;

    // Забавный вызов)))
    // sort(&my_arr.begin().get(), &my_arr.end().get(), [](int a, int b)
    //      { return a < b; });

    // Быстрое решение
    sort(my_arr.begin_ptr(), my_arr.end_ptr(), [](int a, int b)
         { return a < b; });








    // Для такого вызова нужно написать другой шаблон для sort(), но тогда нужно для итератора перегрузить операции сложения итераторов и
    // перегрузить (+/- n)
    /*
    template <typename RandomIt, typename Compare>
    void sort(RandomIt first, RandomIt last, Compare comp)
    {
        if (first == last || first + 1 == last) {
            return;
        }
        quicksort(first, last, comp);
    }
    */

    // sort(my_arr.begin(), my_arr.end(), [](int a, int b)
    //      { return a < b; });






    std::cout << "Отсортированный массив (Array): ";
    for (int num : my_arr)
        std::cout << num << " ";
    std::cout << std::endl;
}
