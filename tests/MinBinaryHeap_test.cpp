#include <algorithm>  // std::sort
#include <vector>     // std::vector

#include "gtest/gtest.h"
#include "priority_queue/BinaryHeap.h"

using namespace heap;

// The fixture for testing class Foo.
class BinaryMinHeapTest : public ::testing::Test {
protected:
    BinaryMinHeapTest() {
        std::sort(test_vector_sorted.begin(), test_vector_sorted.end());
    }

    std::vector<int> test_vector = {30, 1, 50, 20, 40, 60, 100};
    std::vector<int> test_vector_sorted = test_vector;

    BinaryHeap<int, false> empty_min_heap = make_min_heap<false, int>();
    BinaryHeap<int, false> min_heap = make_min_heap<false, int>(test_vector);

    std::vector<int> already_min_seed = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    BinaryHeap<int, true> already_min_heap = make_min_heap<true, int>(already_min_seed);
};

TEST_F(BinaryMinHeapTest, WorksIfEmpty) {
    ASSERT_TRUE(empty_min_heap.empty());

    for (const auto& v : test_vector) {
        empty_min_heap.push(v);
    }

    ASSERT_FALSE(empty_min_heap.empty());
    ASSERT_EQ(empty_min_heap.size(), test_vector.size());

    for (const auto& v : test_vector_sorted) {
        const auto x = empty_min_heap.top();
        ASSERT_EQ(x, v);
        empty_min_heap.pop();
    }
}

TEST_F(BinaryMinHeapTest, WorksIfFull) {
    ASSERT_FALSE(min_heap.empty());
    ASSERT_EQ(min_heap.size(), test_vector.size());

    for (const auto& v : test_vector_sorted) {
        const auto x = min_heap.top();
        ASSERT_EQ(x, v);
        min_heap.pop();
    }
}

TEST_F(BinaryMinHeapTest, WorksIfAlreadyHeap) {
    ASSERT_FALSE(already_min_heap.empty());
    ASSERT_EQ(already_min_heap.size(), already_min_seed.size());

    for (const auto& v : already_min_seed) {
        const auto x = already_min_heap.top();
        ASSERT_EQ(x, v);
        already_min_heap.pop();
    }
}