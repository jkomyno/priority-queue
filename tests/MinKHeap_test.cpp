#include <algorithm>  // std::sort
#include <vector>     // std::vector

#include "gtest/gtest.h"
#include "priority_queue/KHeap.h"

using namespace heap;

/////////////////
// K=4
/////////////////

class K4MinHeapTest : public ::testing::Test {
protected:
    K4MinHeapTest() {
        std::sort(test_vector_sorted.begin(), test_vector_sorted.end());
    }

    std::vector<int> test_vector = {30, 1, 50, 20, 40, 60, 100};
    std::vector<int> test_vector_sorted = test_vector;

    KHeap<4, int, false> empty_min_heap = make_min_k_heap<4, false, int>();
    KHeap<4, int, false> min_heap = make_min_k_heap<4, false, int>(test_vector);

    std::vector<int> already_min_seed = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    KHeap<4, int, true> already_min_heap = make_min_k_heap<4, true, int>(already_min_seed);
};

TEST_F(K4MinHeapTest, WorksIfEmpty) {
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

TEST_F(K4MinHeapTest, WorksIfFull) {
    ASSERT_FALSE(min_heap.empty());
    ASSERT_EQ(min_heap.size(), test_vector.size());

    for (const auto& v : test_vector_sorted) {
        const auto x = min_heap.top();
        ASSERT_EQ(x, v);
        min_heap.pop();
    }
}

TEST_F(K4MinHeapTest, WorksIfAlreadyHeap) {
    ASSERT_FALSE(already_min_heap.empty());
    ASSERT_EQ(already_min_heap.size(), already_min_seed.size());

    for (const auto& v : already_min_seed) {
        const auto x = already_min_heap.top();
        ASSERT_EQ(x, v);
        already_min_heap.pop();
    }
}

/////////////////
// K=8
/////////////////

class K8MinHeapTest : public ::testing::Test {
protected:
    K8MinHeapTest() {
        std::sort(test_vector_sorted.begin(), test_vector_sorted.end());
    }

    std::vector<int> test_vector = {30, 1, 50, 20, 40, 60, 100};
    std::vector<int> test_vector_sorted = test_vector;

    KHeap<8, int, false> empty_min_heap = make_min_k_heap<8, false, int>();
    KHeap<8, int, false> min_heap = make_min_k_heap<8, false, int>(std::vector<int>(test_vector));

    std::vector<int> already_min_seed = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    KHeap<8, int, true> already_min_heap =
        make_min_k_heap<8, true, int>(std::vector<int>(already_min_seed));
};

TEST_F(K8MinHeapTest, WorksIfEmpty) {
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

TEST_F(K8MinHeapTest, WorksIfFull) {
    ASSERT_FALSE(min_heap.empty());
    ASSERT_EQ(min_heap.size(), test_vector.size());

    for (const auto& v : test_vector_sorted) {
        const auto x = min_heap.top();
        ASSERT_EQ(x, v);
        min_heap.pop();
    }
}

TEST_F(K8MinHeapTest, WorksIfAlreadyHeap) {
    ASSERT_FALSE(already_min_heap.empty());
    ASSERT_EQ(already_min_heap.size(), already_min_seed.size());

    for (const auto& v : already_min_seed) {
        const auto x = already_min_heap.top();
        ASSERT_EQ(x, v);
        already_min_heap.pop();
    }
}