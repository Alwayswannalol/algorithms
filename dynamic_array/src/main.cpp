#include "Array.h"

#include <iostream>

int main()
{
    Array<int> arr;
    for (int i = 0; i < 20; i++) {
        arr.insert(i);
    }

    std::cout << "size arr: " << arr.size() << std::endl;

    for (auto it = arr.begin(); it != arr.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    auto it = arr.iterator();
    std::cout << "first value " << *it << std::endl;
    std::cout << "prefix " << *(++it) << std::endl;
    std::cout << "postfix " << *(it++) << std::endl;
    std::cout << "after postfix " << *it << std::endl;

    *it = 123;
    arr[3] = 124;
    std::cout << "arr after changing arr[2] and arr[3]" << std::endl;
    for (const auto &elem : arr)
    {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // test of ConstIterator
    const Array<int> const_arr;
    auto const_it = const_arr.iterator();
    auto test_it = arr.cbegin();

    arr.insert(5, 145);
    std::cout << "arr after insert(5, 145)" << std::endl;
    for (const auto &elem : arr)
    {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    arr.remove(2);
    std::cout << "arr after remove(2)" << std::endl;
    for (const auto &elem : arr)
    {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    return 0;
}
