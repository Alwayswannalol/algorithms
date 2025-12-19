#include "QuickSort.h"
#include "Array.h"
#include <gtest/gtest.h>
#include <algorithm>
#include <vector>
#include <random>
#include <string>
#include <climits>
#include <cstdlib>
#include <ctime>

class QuickSortBasicTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::srand(std::time(nullptr));
    }
};

TEST_F(QuickSortBasicTest, EmptyArray) {
    int arr[] = {};
    sort(arr, arr, std::less<int>());
    SUCCEED();
}

TEST_F(QuickSortBasicTest, SingleElement) {
    int arr[] = {42};
    sort(arr, arr + 1, std::less<int>());
    EXPECT_EQ(arr[0], 42);
}

TEST_F(QuickSortBasicTest, TwoElementsSorted) {
    int arr[] = {1, 2};
    sort(arr, arr + 2, std::less<int>());
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
}

TEST_F(QuickSortBasicTest, TwoElementsUnsorted) {
    int arr[] = {2, 1};
    sort(arr, arr + 2, std::less<int>());
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
}

TEST_F(QuickSortBasicTest, ThreeElements) {
    int arr[] = {3, 1, 2};
    sort(arr, arr + 3, std::less<int>());
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
}

TEST_F(QuickSortBasicTest, AlreadySorted) {
    int arr[] = {1, 2, 3, 4, 5};
    sort(arr, arr + 5, std::less<int>());
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(arr[i], i + 1);
    }
}

TEST_F(QuickSortBasicTest, ReverseSorted) {
    int arr[] = {5, 4, 3, 2, 1};
    sort(arr, arr + 5, std::less<int>());
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(arr[i], i + 1);
    }
}

TEST_F(QuickSortBasicTest, WithDuplicates) {
    int arr[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    int expected[] = {1, 1, 2, 3, 3, 4, 5, 5, 6, 9};
    sort(arr, arr + 10, std::less<int>());
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(arr[i], expected[i]);
    }
}

class QuickSortLargeTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::srand(std::time(nullptr));
    }
};

TEST_F(QuickSortLargeTest, LargeRandomArray) {
    const int N = 1000;
    std::vector<int> arr(N);
    
    for (int i = 0; i < N; ++i) {
        arr[i] = std::rand() % 20001 - 10000;
    }
    
    std::vector<int> expected = arr;
    std::sort(expected.begin(), expected.end());
    
    sort(arr.data(), arr.data() + N, std::less<int>());
    
    for (int i = 0; i < N; ++i) {
        EXPECT_EQ(arr[i], expected[i]) << "Mismatch at index " << i;
    }
}

TEST_F(QuickSortLargeTest, VeryLargeArray) {
    const int N = 10000;
    std::vector<int> arr(N);
    
    for (int i = 0; i < N; ++i) {
        arr[i] = std::rand() % 100001 - 50000;
    }
    
    std::vector<int> expected = arr;
    std::sort(expected.begin(), expected.end());
    
    sort(arr.data(), arr.data() + N, std::less<int>());
    
    for (int i = 0; i < N - 1; ++i) {
        EXPECT_LE(arr[i], arr[i + 1]) << "Not sorted at index " << i;
    }
}

TEST_F(QuickSortLargeTest, AllSameElements) {
    const int N = 100;
    std::vector<int> arr(N, 42);
    
    sort(arr.data(), arr.data() + N, std::less<int>());
    
    for (int i = 0; i < N; ++i) {
        EXPECT_EQ(arr[i], 42);
    }
}

TEST_F(QuickSortLargeTest, LargeReverseSorted) {
    const int N = 500;
    std::vector<int> arr(N);
    
    for (int i = 0; i < N; ++i) {
        arr[i] = N - i;
    }
    
    sort(arr.data(), arr.data() + N, std::less<int>());
    
    for (int i = 0; i < N; ++i) {
        EXPECT_EQ(arr[i], i + 1);
    }
}

class QuickSortTypesTest : public ::testing::Test {};

TEST_F(QuickSortTypesTest, DoubleArray) {
    double arr[] = {3.14, 1.41, 2.71, 0.57, 1.73};
    double expected[] = {0.57, 1.41, 1.73, 2.71, 3.14};
    
    sort(arr, arr + 5, std::less<double>());
    
    for (int i = 0; i < 5; ++i) {
        EXPECT_DOUBLE_EQ(arr[i], expected[i]);
    }
}

TEST_F(QuickSortTypesTest, FloatArray) {
    float arr[] = {3.14f, 1.41f, 2.71f, 0.57f, 1.73f};
    float expected[] = {0.57f, 1.41f, 1.73f, 2.71f, 3.14f};
    
    sort(arr, arr + 5, std::less<float>());
    
    for (int i = 0; i < 5; ++i) {
        EXPECT_FLOAT_EQ(arr[i], expected[i]);
    }
}

TEST_F(QuickSortTypesTest, StringArray) {
    std::string arr[] = {"banana", "apple", "cherry", "date", "elderberry"};
    std::string expected[] = {"apple", "banana", "cherry", "date", "elderberry"};
    
    sort(arr, arr + 5, std::less<std::string>());
    
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(arr[i], expected[i]);
    }
}

TEST_F(QuickSortTypesTest, CustomStruct) {
    struct Person {
        std::string name;
        int age;
        
        bool operator<(const Person& other) const {
            return age < other.age;
        }
    };
    
    Person people[] = {
        {"Alice", 30},
        {"Bob", 25},
        {"Charlie", 35},
        {"David", 20}
    };
    
    sort(people, people + 4, std::less<Person>());
    
    EXPECT_EQ(people[0].name, "David");
    EXPECT_EQ(people[0].age, 20);
    EXPECT_EQ(people[1].name, "Bob");
    EXPECT_EQ(people[1].age, 25);
    EXPECT_EQ(people[2].name, "Alice");
    EXPECT_EQ(people[2].age, 30);
    EXPECT_EQ(people[3].name, "Charlie");
    EXPECT_EQ(people[3].age, 35);
}

TEST_F(QuickSortTypesTest, CustomComparator) {
    int arr[] = {5, 2, 8, 1, 9};
    int expected[] = {9, 8, 5, 2, 1};
    
    sort(arr, arr + 5, std::greater<int>());
    
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(arr[i], expected[i]);
    }
}

TEST_F(QuickSortTypesTest, LambdaComparator) {
    std::vector<std::string> arr = {"banana", "apple", "Cherry", "Date"};
    std::vector<std::string> expected = {"apple", "banana", "Cherry", "Date"};
    
    auto case_insensitive = [](const std::string& a, const std::string& b) {
        std::string a_lower, b_lower;
        std::transform(a.begin(), a.end(), std::back_inserter(a_lower), ::tolower);
        std::transform(b.begin(), b.end(), std::back_inserter(b_lower), ::tolower);
        return a_lower < b_lower;
    };
    
    sort(arr.data(), arr.data() + arr.size(), case_insensitive);
    
    for (size_t i = 0; i < arr.size(); ++i) {
        EXPECT_EQ(arr[i], expected[i]);
    }
}

class QuickSortArrayTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::srand(std::time(nullptr));
    }
};

TEST_F(QuickSortArrayTest, ArrayInt) {
    Array<int> arr;
    for (int i = 0; i < 100; ++i) {
        arr.insert(i);
    }
    
    for (int i = 99; i > 0; --i) {
        int j = std::rand() % (i + 1);
        std::swap(arr[i], arr[j]);
    }
    
    sort(arr.begin_ptr(), arr.end_ptr(), std::less<int>());
    
    for (int i = 0; i < 99; ++i) {
        EXPECT_LE(arr[i], arr[i + 1]) << "Not sorted at index " << i;
    }
    
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(arr[i], i) << "Missing element " << i;
    }
}

TEST_F(QuickSortArrayTest, ArrayString) {
    Array<std::string> arr;
    arr.insert("zebra");
    arr.insert("apple");
    arr.insert("cherry");
    arr.insert("banana");
    arr.insert("date");
    
    sort(arr.begin_ptr(), arr.end_ptr(), std::less<std::string>());
    
    EXPECT_EQ(arr[0], "apple");
    EXPECT_EQ(arr[1], "banana");
    EXPECT_EQ(arr[2], "cherry");
    EXPECT_EQ(arr[3], "date");
    EXPECT_EQ(arr[4], "zebra");
}

class QuickSortCornerCasesTest : public ::testing::Test {};

TEST_F(QuickSortCornerCasesTest, MinMaxValues) {
    int arr[] = {INT_MAX, INT_MIN, 0, -1, 1};
    int expected[] = {INT_MIN, -1, 0, 1, INT_MAX};
    
    sort(arr, arr + 5, std::less<int>());
    
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(arr[i], expected[i]);
    }
}

TEST_F(QuickSortCornerCasesTest, ManySwapsNeeded) {
    const int N = 100;
    std::vector<int> arr(N);
    
    for (int i = 0; i < N / 2; ++i) {
        arr[2 * i] = N - i - 1;
        arr[2 * i + 1] = i;
    }
    
    std::vector<int> expected = arr;
    std::sort(expected.begin(), expected.end());
    
    sort(arr.data(), arr.data() + N, std::less<int>());
    
    for (int i = 0; i < N; ++i) {
        EXPECT_EQ(arr[i], expected[i]) << "Mismatch at index " << i;
    }
}

TEST_F(QuickSortCornerCasesTest, InsertionSortThreshold) {
    const int N = INSERTION_SORT_QUANT;
    std::vector<int> arr(N);
    
    for (int i = 0; i < N; ++i) {
        arr[i] = std::rand() % 101;
    }
    
    std::vector<int> expected = arr;
    std::sort(expected.begin(), expected.end());
    
    sort(arr.data(), arr.data() + N, std::less<int>());
    
    for (int i = 0; i < N; ++i) {
        EXPECT_EQ(arr[i], expected[i]) << "Mismatch at index " << i;
    }
}

TEST_F(QuickSortCornerCasesTest, JustAboveInsertionSortThreshold) {
    const int N = INSERTION_SORT_QUANT + 1;
    std::vector<int> arr(N);
    
    for (int i = 0; i < N; ++i) {
        arr[i] = std::rand() % 101;
    }
    
    std::vector<int> expected = arr;
    std::sort(expected.begin(), expected.end());
    
    sort(arr.data(), arr.data() + N, std::less<int>());
    
    for (int i = 0; i < N; ++i) {
        EXPECT_EQ(arr[i], expected[i]) << "Mismatch at index " << i;
    }
}

TEST_F(QuickSortCornerCasesTest, NotStableSort) {
    struct Item {
        int key;
        int value;
        
        bool operator<(const Item& other) const {
            return key < other.key;
        }
    };
    
    Item items[] = {
        {1, 100},
        {2, 200},
        {1, 300},
        {3, 400},
        {2, 500}
    };
    
    sort(items, items + 5, std::less<Item>());
    
    for (int i = 0; i < 4; ++i) {
        EXPECT_LE(items[i].key, items[i + 1].key);
    }
}

TEST_F(QuickSortCornerCasesTest, MedianOfThreeCheck) {
    int a = 1, b = 2, c = 3;
    int* med = median_of_three(&a, &b, &c, std::less<int>());
    EXPECT_EQ(*med, 2);
    
    a = 3; b = 2; c = 1;
    med = median_of_three(&a, &b, &c, std::less<int>());
    EXPECT_EQ(*med, 2);
    
    a = 2; b = 3; c = 1;
    med = median_of_three(&a, &b, &c, std::less<int>());
    EXPECT_EQ(*med, 2);
    
    a = 2; b = 1; c = 3;
    med = median_of_three(&a, &b, &c, std::less<int>());
    EXPECT_EQ(*med, 2);
    
    a = 3; b = 1; c = 2;
    med = median_of_three(&a, &b, &c, std::less<int>());
    EXPECT_EQ(*med, 2);
    
    a = 1; b = 3; c = 2;
    med = median_of_three(&a, &b, &c, std::less<int>());
    EXPECT_EQ(*med, 2);
}

TEST_F(QuickSortCornerCasesTest, PartitionCheck) {
    int arr[] = {5, 3, 8, 1, 2, 7, 4, 6};
    int* pivot = &arr[3];
    
    int* p = partition(arr, arr + 8, pivot, std::less<int>());
    for (int i = 0; i < p - arr; ++i) {
        EXPECT_LE(arr[i], *p);
    }
    for (int i = p - arr + 1; i < 8; ++i) {
        EXPECT_GE(arr[i], *p);
    }
}

TEST_F(QuickSortCornerCasesTest, PerformanceTest) {
    const int N = 100000;
    std::vector<int> arr(N);
    
    for (int i = 0; i < N; ++i) {
        arr[i] = std::rand() % 1000001;
    }
    
    EXPECT_NO_THROW({
        sort(arr.data(), arr.data() + N, std::less<int>());
    });
    
    for (int i = 0; i < N - 1; ++i) {
        EXPECT_LE(arr[i], arr[i + 1]) << "Not sorted at index " << i;
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}