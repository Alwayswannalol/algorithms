#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <random>

#include "QuickSort.h"

std::vector<int> generate_random_array(size_t size) {
    std::vector<int> arr(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-10000, 10000);
    
    for (size_t i = 0; i < size; ++i) {
        arr[i] = dis(gen);
    }
    return arr;
}

void test_quicksort_no_insertion(std::ofstream& csv_file) {
    csv_file << "size,time_ms\n";
    
    for (int size = 2; size <= 1000; ++size) {
        double total_time = 0.0;
        
        for (int iteration = 0; iteration < 3; ++iteration) {
            std::vector<int> arr = generate_random_array(size);
            std::vector<int> arr_copy = arr;
            
            auto start = std::chrono::high_resolution_clock::now();
            
            quicksort_no_insertion(arr.data(), arr.data() + arr.size(), std::less<int>());
            
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
            
            total_time += duration.count() / 1e6;
            
            if (!std::is_sorted(arr.begin(), arr.end())) {
                std::cerr << "Ошибка сортировки для size=" << size << ", iteration=" << iteration << "\n";
            }
        }
        
        double avg_time = total_time / 3.0;
        csv_file << size << "," << avg_time << "\n";
        
        if (size % 100 == 0) {
            std::cout << "Тестирование quicksort_no_insertion: size=" << size 
                      << ", avg_time=" << avg_time << " ms\n";
        }
    }
}

void test_insertion_sort_only(std::ofstream& csv_file) {
    csv_file << "size,time_ms\n";
    
    for (int size = 2; size <= 1000; ++size) {
        double total_time = 0.0;
        
        for (int iteration = 0; iteration < 3; ++iteration) {
            std::vector<int> arr = generate_random_array(size);
            std::vector<int> arr_copy = arr;
            
            auto start = std::chrono::high_resolution_clock::now();
            
            insertion_sort(arr.data(), arr.data() + arr.size(), std::less<int>());
            
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
            
            total_time += duration.count() / 1e6;
            
            if (!std::is_sorted(arr.begin(), arr.end())) {
                std::cerr << "Ошибка сортировки insertion sort для size=" << size 
                          << ", iteration=" << iteration << "\n";
            }
        }
        
        double avg_time = total_time / 3.0;
        csv_file << size << "," << avg_time << "\n";
        
        if (size % 100 == 0) {
            std::cout << "Тестирование insertion_sort_only: size=" << size 
                      << ", avg_time=" << avg_time << " ms\n";
        }
    }
}

int main()
{
    std::cout << "Начало тестирования алгоритмов сортировки...\n";

    std::ofstream file1("quicksort_no_insertion.csv");
    if (file1.is_open())
    {
        std::cout << "\nТестирование быстрой сортировки (без insertion sort)...\n";
        test_quicksort_no_insertion(file1);
        file1.close();
        std::cout << "Результаты сохранены в quicksort_no_insertion.csv\n";
    }
    else
    {
        std::cerr << "Не удалось открыть файл quicksort_no_insertion.csv\n";
    }

    std::ofstream file2("insertion_sort_only.csv");
    if (file2.is_open())
    {
        std::cout << "\nТестирование только insertion sort...\n";
        test_insertion_sort_only(file2);
        file2.close();
        std::cout << "Результаты сохранены в insertion_sort_only.csv\n";
    }
    else
    {
        std::cerr << "Не удалось открыть файл insertion_sort_only.csv\n";
    }

    std::cout << "\nТестирование завершено. Файлы CSV созданы:\n";
    std::cout << "1. quicksort_no_insertion.csv - чистая быстрая сортировка\n";
    std::cout << "2. insertion_sort_only.csv - только сортировка вставками\n";

    return 0;
}