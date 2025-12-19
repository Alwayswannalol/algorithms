#include "Array.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <algorithm>

class ArrayIntTest : public ::testing::Test {
protected:
    void SetUp() override {
        arr.insert(1);
        arr.insert(2);
        arr.insert(3);
    }
    
    Array<int> arr;
};

TEST_F(ArrayIntTest, DefaultConstructor) {
    Array<int> a;
    EXPECT_EQ(a.size(), 0);
}

TEST_F(ArrayIntTest, ConstructorWithCapacity) {
    Array<int> a(32);
    EXPECT_EQ(a.size(), 0);
    for (int i = 0; i < 32; ++i) {
        a.insert(i);
    }
    EXPECT_EQ(a.size(), 32);
}

TEST_F(ArrayIntTest, CopyConstructor) {
    Array<int> copy(arr);
    EXPECT_EQ(copy.size(), 3);
    EXPECT_EQ(copy[0], 1);
    EXPECT_EQ(copy[1], 2);
    EXPECT_EQ(copy[2], 3);
    
    arr[0] = 100;
    EXPECT_EQ(copy[0], 1);
}

TEST_F(ArrayIntTest, MoveConstructor) {
    Array<int> moved(std::move(arr));
    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved[0], 1);
    EXPECT_EQ(moved[1], 2);
    EXPECT_EQ(moved[2], 3);
    EXPECT_EQ(arr.size(), 0);
}

TEST_F(ArrayIntTest, InsertAtEnd) {
    int index = arr.insert(4);
    EXPECT_EQ(index, 3);
    EXPECT_EQ(arr.size(), 4);
    EXPECT_EQ(arr[3], 4);
}

TEST_F(ArrayIntTest, InsertAtIndex) {
    int index = arr.insert(1, 99);
    EXPECT_EQ(index, 1);
    EXPECT_EQ(arr.size(), 4);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 99);
    EXPECT_EQ(arr[2], 2);
    EXPECT_EQ(arr[3], 3);
}

TEST_F(ArrayIntTest, InsertAtIndexZero) {
    int index = arr.insert(0, 99);
    EXPECT_EQ(index, 0);
    EXPECT_EQ(arr[0], 99);
    EXPECT_EQ(arr[1], 1);
}

TEST_F(ArrayIntTest, InsertAtEndAfterGrowth) {
    Array<int> a;
    for (int i = 0; i < 16; ++i) {
        a.insert(i);
    }
    EXPECT_EQ(a.size(), 16);
    
    a.insert(16);
    EXPECT_EQ(a.size(), 17);
    EXPECT_EQ(a[16], 16);
}

TEST_F(ArrayIntTest, Remove) {
    arr.remove(1);
    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 3);
}

TEST_F(ArrayIntTest, RemoveFirst) {
    arr.remove(0);
    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0], 2);
    EXPECT_EQ(arr[1], 3);
}

TEST_F(ArrayIntTest, RemoveLast) {
    arr.remove(2);
    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
}

TEST_F(ArrayIntTest, OperatorBracketRead) {
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
}

TEST_F(ArrayIntTest, OperatorBracketWrite) {
    arr[1] = 99;
    EXPECT_EQ(arr[1], 99);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[2], 3);
}

TEST_F(ArrayIntTest, Size) {
    EXPECT_EQ(arr.size(), 3);
    arr.insert(4);
    EXPECT_EQ(arr.size(), 4);
    arr.remove(0);
    EXPECT_EQ(arr.size(), 3);
}

TEST_F(ArrayIntTest, Iterator) {
    std::vector<int> collected;
    for (auto it = arr.iterator(); ; ) {
        collected.push_back(it.get());
        if (!it.hasNext()) break;
        it.next();
    }
    
    EXPECT_EQ(collected.size(), 3);
    EXPECT_EQ(collected[0], 1);
    EXPECT_EQ(collected[1], 2);
    EXPECT_EQ(collected[2], 3);
}

TEST_F(ArrayIntTest, IteratorHasNextBehavior) {
    Array<int> single;
    single.insert(42);
    
    auto it = single.iterator();
    EXPECT_EQ(it.get(), 42);
    EXPECT_FALSE(it.hasNext());
    
    Array<int> empty;
    auto it2 = empty.iterator();
    EXPECT_FALSE(it2.hasNext());
}

TEST_F(ArrayIntTest, IteratorWithOperators) {
    auto it = arr.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    it++;
    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_EQ(it, arr.end());
}

TEST_F(ArrayIntTest, ReverseIterator) {
    std::vector<int> collected;
    for (auto it = arr.reverseIterator(); ; ) {
        collected.push_back(it.get());
        if (!it.hasNext()) break;
        it.next();
    }
    
    EXPECT_EQ(collected.size(), 3);
    EXPECT_EQ(collected[0], 3);
    EXPECT_EQ(collected[1], 2);
    EXPECT_EQ(collected[2], 1);
}

TEST_F(ArrayIntTest, ReverseIteratorHasNextBehavior) {
    Array<int> single;
    single.insert(42);
    
    auto it = single.reverseIterator();
    EXPECT_EQ(it.get(), 42);
    EXPECT_FALSE(it.hasNext());
}

TEST_F(ArrayIntTest, RangeBasedForLoop) {
    int sum = 0;
    for (int val : arr) {
        sum += val;
    }
    EXPECT_EQ(sum, 6);
}

TEST_F(ArrayIntTest, RangeBasedForLoopEmpty) {
    Array<int> empty;
    int count = 0;
    for (int val : empty) {
        ++count;
    }
    EXPECT_EQ(count, 0);
}

TEST_F(ArrayIntTest, CopyAssignment) {
    Array<int> copy;
    copy = arr;
    EXPECT_EQ(copy.size(), 3);
    EXPECT_EQ(copy[0], 1);
    EXPECT_EQ(copy[1], 2);
    EXPECT_EQ(copy[2], 3);
}

TEST_F(ArrayIntTest, MoveAssignment) {
    Array<int> moved;
    moved = std::move(arr);
    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved[0], 1);
    EXPECT_EQ(moved[1], 2);
    EXPECT_EQ(moved[2], 3);
    EXPECT_EQ(arr.size(), 0);
}

TEST_F(ArrayIntTest, SelfAssignment) {
    Array<int>* ptr = &arr;
    arr = *ptr;
    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr[0], 1);
}

class ArrayStringTest : public ::testing::Test {
protected:
    void SetUp() override {
        arr.insert("one");
        arr.insert("two");
        arr.insert("three");
    }
    
    Array<std::string> arr;
};

TEST_F(ArrayStringTest, InsertAndAccess) {
    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr[0], "one");
    EXPECT_EQ(arr[1], "two");
    EXPECT_EQ(arr[2], "three");
}

TEST_F(ArrayStringTest, InsertAtIndex) {
    int index = arr.insert(1, "inserted");
    EXPECT_EQ(index, 1);
    EXPECT_EQ(arr.size(), 4);
    EXPECT_EQ(arr[0], "one");
    EXPECT_EQ(arr[1], "inserted");
    EXPECT_EQ(arr[2], "two");
    EXPECT_EQ(arr[3], "three");
}

TEST_F(ArrayStringTest, Remove) {
    arr.remove(0);
    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0], "two");
    EXPECT_EQ(arr[1], "three");
}

TEST_F(ArrayStringTest, CopyConstructor) {
    Array<std::string> copy(arr);
    EXPECT_EQ(copy.size(), 3);
    EXPECT_EQ(copy[0], "one");
    
    arr[0] = "modified";
    EXPECT_EQ(copy[0], "one");
}

TEST_F(ArrayStringTest, MoveSemantics) {
    std::string longString(100, 'a');
    Array<std::string> a;
    a.insert(longString);
    
    Array<std::string> b;
    b = std::move(a);
    EXPECT_EQ(b.size(), 1);
    EXPECT_EQ(b[0], longString);
    EXPECT_EQ(a.size(), 0);
}

TEST_F(ArrayStringTest, Iterator) {
    std::vector<std::string> expected = {"one", "two", "three"};
    std::vector<std::string> collected;
    
    for (auto it = arr.iterator(); ; ) {
        collected.push_back(it.get());
        if (!it.hasNext()) break;
        it.next();
    }
    
    EXPECT_EQ(collected.size(), 3);
    EXPECT_EQ(collected[0], "one");
    EXPECT_EQ(collected[1], "two");
    EXPECT_EQ(collected[2], "three");
}

TEST_F(ArrayStringTest, RangeBasedForLoop) {
    std::string concatenated;
    for (const auto& str : arr) {
        concatenated += str;
    }
    EXPECT_EQ(concatenated, "onetwothree");
}

TEST_F(ArrayStringTest, MultipleInsertionsAndDeletions) {
    Array<std::string> a;
    
    for (int i = 0; i < 100; ++i) {
        a.insert("string_" + std::to_string(i));
    }
    EXPECT_EQ(a.size(), 100);
    
    for (int i = 49; i >= 0; --i) {
        a.remove(i * 2);
    }
    EXPECT_EQ(a.size(), 50);
    
    for (int i = 0; i < 50; ++i) {
        EXPECT_EQ(a[i], "string_" + std::to_string(i * 2 + 1));
    }
}

TEST_F(ArrayStringTest, SelfAssignment) {
    Array<std::string>* ptr = &arr;
    arr = *ptr;
    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr[0], "one");
}

TEST(ArrayEdgeCasesTest, EmptyArray) {
    Array<int> arr;
    EXPECT_EQ(arr.size(), 0);
    EXPECT_EQ(arr.begin(), arr.end());
    
    auto it = arr.iterator();
    EXPECT_FALSE(it.hasNext());
    
    auto rit = arr.reverseIterator();
    EXPECT_FALSE(rit.hasNext());
}

TEST(ArrayEdgeCasesTest, InsertIntoEmpty) {
    Array<std::string> arr;
    int index = arr.insert("first");
    EXPECT_EQ(index, 0);
    EXPECT_EQ(arr.size(), 1);
    EXPECT_EQ(arr[0], "first");
}

TEST(ArrayEdgeCasesTest, RemoveAllElements) {
    Array<int> arr;
    arr.insert(1);
    arr.insert(2);
    arr.insert(3);
    
    arr.remove(0);
    arr.remove(0);
    arr.remove(0);
    
    EXPECT_EQ(arr.size(), 0);
    
    arr.insert(42);
    EXPECT_EQ(arr.size(), 1);
    EXPECT_EQ(arr[0], 42);
}

TEST(ArrayEdgeCasesTest, LargeNumberOfElements) {
    Array<int> arr;
    const int N = 10000;
    
    for (int i = 0; i < N; ++i) {
        arr.insert(i);
    }
    
    EXPECT_EQ(arr.size(), N);
    
    for (int i = 0; i < N; ++i) {
        EXPECT_EQ(arr[i], i);
    }
    
    int count = 0;
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        EXPECT_EQ(*it, count++);
    }
    EXPECT_EQ(count, N);
}

TEST(ArrayEdgeCasesTest, InsertAtBoundaries) {
    Array<int> arr;
    arr.insert(1);
    arr.insert(2);
    
    arr.insert(0, 0);
    EXPECT_EQ(arr[0], 0);
    EXPECT_EQ(arr[1], 1);
    EXPECT_EQ(arr[2], 2);
    
    arr.insert(arr.size(), 3);
    EXPECT_EQ(arr[3], 3);
}

TEST(ArrayEdgeCasesTest, ConstAccess) {
    Array<int> arr;
    arr.insert(1);
    arr.insert(2);
    
    const Array<int>& const_arr = arr;
    EXPECT_EQ(const_arr[0], 1);
    EXPECT_EQ(const_arr[1], 2);
    EXPECT_EQ(const_arr.size(), 2);
    
    int sum = 0;
    for (auto it = const_arr.begin(); it != const_arr.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 3);
}

TEST(ArrayExceptionTest, BadAllocInConstructor) {
    try {
        Array<int> arr(1000000000LL);
        SUCCEED();
    } catch (const std::bad_alloc&) {
        SUCCEED();
    }
}

TEST(ArrayConstIteratorTest, ConstIteratorAccess) {
    Array<int> arr;
    arr.insert(1);
    arr.insert(2);
    arr.insert(3);
    
    const Array<int>& const_arr = arr;
    
    auto it = const_arr.iterator();
    EXPECT_EQ(it.get(), 1);
    
    std::vector<int> collected;
    for (auto cit = const_arr.cbegin(); cit != const_arr.cend(); ++cit) {
        collected.push_back(*cit);
    }
    
    EXPECT_EQ(collected.size(), 3);
    EXPECT_EQ(collected[0], 1);
    EXPECT_EQ(collected[1], 2);
    EXPECT_EQ(collected[2], 3);
}

TEST(ArrayConstIteratorTest, ReverseConstIterator) {
    Array<int> arr;
    arr.insert(1);
    arr.insert(2);
    arr.insert(3);
    
    const Array<int>& const_arr = arr;
    
    auto rit = const_arr.reverseIterator();
    EXPECT_EQ(rit.get(), 3);
    
    std::vector<int> collected;
    for (auto it = const_arr.reverseIterator(); ; ) {
        collected.push_back(it.get());
        if (!it.hasNext()) break;
        it.next();
    }
    
    EXPECT_EQ(collected.size(), 3);
    EXPECT_EQ(collected[0], 3);
    EXPECT_EQ(collected[1], 2);
    EXPECT_EQ(collected[2], 1);
}

TEST(ArrayDestructorTest, DestructorCalls) {
    static int destructor_count = 0;
    
    struct TestStruct {
        ~TestStruct() {
            destructor_count++;
        }
        int value;
    };
    
    destructor_count = 0;
    {
        Array<TestStruct> arr;
        arr.insert(TestStruct{1});
        arr.insert(TestStruct{2});
        arr.insert(TestStruct{3});
        
        arr.remove(1);
        EXPECT_GE(destructor_count, 1);
        
        arr.insert(1, TestStruct{99});
    }
    EXPECT_GE(destructor_count, 4);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}