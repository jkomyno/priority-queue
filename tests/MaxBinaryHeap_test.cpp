#include <algorithm>  // std::sort
#include <vector>     // std::vector

#include "gtest/gtest.h"
#include "priority_queue/BinaryHeap.h"

using namespace heap;

// The fixture for testing class Foo.
class BinaryMaxHeapTest : public ::testing::Test {
protected:
    BinaryMaxHeapTest() {
        std::sort(test_vector_sorted.begin(), test_vector_sorted.end(), std::greater<>());
    }

    std::vector<int> test_vector = {30, 1, 50, 20, 40, 60, 100};
    std::vector<int> test_vector_sorted = test_vector;

    BinaryHeap<int, false> empty_max_heap = make_max_heap<false, int>();
    BinaryHeap<int, false> max_heap = make_max_heap<false, int>(test_vector);

    std::vector<int> already_max_seed = {8, 7, 6, 5, 4, 3, 2, 1, 0};
    BinaryHeap<int, true> already_max_heap = make_max_heap<true, int>(already_max_seed);
};

TEST_F(BinaryMaxHeapTest, WorksIfEmpty) {
    ASSERT_TRUE(empty_max_heap.empty());

    for (const auto& v : test_vector) {
        empty_max_heap.push(v);
    }

    ASSERT_FALSE(empty_max_heap.empty());
    ASSERT_EQ(empty_max_heap.size(), test_vector.size());

    for (const auto& v : test_vector_sorted) {
        const auto x = empty_max_heap.top();
        ASSERT_EQ(x, v);
        empty_max_heap.pop();
    }
}

TEST_F(BinaryMaxHeapTest, WorksIfFull) {
    ASSERT_FALSE(max_heap.empty());
    ASSERT_EQ(max_heap.size(), test_vector.size());

    for (const auto& v : test_vector_sorted) {
        const auto x = max_heap.top();
        ASSERT_EQ(x, v);
        max_heap.pop();
    }
}

TEST_F(BinaryMaxHeapTest, WorksIfAlreadyHeap) {
    ASSERT_FALSE(already_max_heap.empty());
    ASSERT_EQ(already_max_heap.size(), already_max_seed.size());

    for (const auto& v : already_max_seed) {
        const auto x = already_max_heap.top();
        ASSERT_EQ(x, v);
        already_max_heap.pop();
    }
}