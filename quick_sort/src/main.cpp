#include <iostream>
#include <vector>

#include "QuickSort.h"

int main()
{
    std::vector<int> arr = {9, 3, 7, 4, 1, 8, 2, 5, 6};

    std::cout << "Исходный массив: ";
    for (int num : arr)
        std::cout << num << " ";
    std::cout << std::endl;

    sort(arr.data(), arr.data() + arr.size(), [](int a, int b)
         { return a < b; });

    std::cout << "Отсортированный массив: ";
    for (int num : arr)
        std::cout << num << " ";
    std::cout << std::endl;
}
