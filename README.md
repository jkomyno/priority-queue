Priority Queue
=================

[![Build Status](https://travis-ci.org/jkomyno/priority-queue.svg?branch=master)](https://travis-ci.org/jkomyno/priority-queue)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](./LICENSE)

This package provides a header-only, generic and dependency-free C++17 implementation of Heaps and Priority Queues.
It exposes two namespaces: `heap` and `priority_queue`.

# Why

> Why did you have to go through the hassle of creating your own Heap and Priority Queue implementations when
the standard library offers `std::make_heap` and the `std::priority_queue container` adaptor?

My answers to this question are multifold:

- The STL doesn't offer any method to update the keys when the elements are stored in the Heap.
Doing that manually can be non-trivial and usually requires at least O(N) time (while this library achieves that faster).
- The STL doesn't provide any way of creating K-ary Heaps or Priority Queues based on K-ary Heaps.
- For University, I was required to implement Prim's algorithm, but wasn't allowed to use `std::make_heap` and `std::priority_queue container`.
- I've used this project as an exercise to enhance my comprehension of C++17 and its best practices, as well as *DRY* principles.

# Heap

The `heap` namespace contains the abstract class [`Heap`](./PriorityQueue/Heap.h) and the concrete implementations [`BinaryHeap`](./PriorityQueue/BinaryHeap.h) (for Binary Heaps)
and [`KHeap`](./PriorityQueue/KHeap.h) (for K-ary Heaps). A Binary Heap can be seen as a complete binary tree which satisfies the heap ordering property.
A K-ary Heap is, on the other hand, a generalization of the Binary Heap in which the nodes have `K` children instead of 2.

Both `BinaryHeap` and `KHeap` are either Min Heaps or Max Heaps depending on comparator used to initialize them,
much like what happens in the STL. To get a Min Heap, the given comparator should be `std::less<>{}`, whereas to get
a Max Heap, the given comparator should be `std::greater<>{}`.

A number of utility factory functions are provided to easily create the type of Heap you want, namely:

- `make_min_heap<bool IsAlreadyHeap = false, typename T>`: create a Min Binary Heap starting from a vector of values.
- `make_max_heap<bool IsAlreadyHeap = false, typename T>`: create a Max Binary Heap starting from a vector of values.
- `make_min_k_heap<std::size_t K, bool IsAlreadyHeap = false, typename T>`: create a Min K-ary Heap starting from a vector of keys and a vector of values.
- `make_max_k_heap<std::size_t K, bool IsAlreadyHeap = false, typename T>`: create a Max K-ary Heap starting from a vector of keys and a vector of values.

Each of these functions supports both copy and move semantics for the given inputs.

Optionally, if you know that the vector you're feeding to the Heap implementation already respects the Heap property
(i.e. `std::is_heap(your_vector)` evaluates to `true`), you can set the template argument `IsAlreadyHeap` to true.
This avoids the `O(N)` time required to build the heap. This template argument is checked at compile time thanks to `if constexpr`.

## Example usage of BinaryHeap

```c++
#include <iostream>
#include "priority_queue/BinaryHeap.h"

int main() {
    std::vector<char> vec { 'b', 'c', 'f', 'a', 'e', 'd' };
    auto min_heap(heap::make_min_heap(std::move(vec)));

    while (!min_heap.empty()) {
        std::cout << "Min: " << min_heap.top() << std::endl;
        min_heap.pop();
    }

    // Output:
    // Min: a
    // Min: b
    // Min: c
    // Min: d
    // Min: e
    // Min: f

    std::cout << std::endl << std::endl;

    auto max_heap(heap::make_max_heap<true, std::size_t>());

    max_heap.push(30);
    max_heap.push(15);
    max_heap.push(5000);
    max_heap.push(50);
    max_heap.push(599);

    while (!max_heap.empty()) {
        std::cout << "Max: " << max_heap.top() << std::endl;
        max_heap.pop();
    }

    // Output:
    // Max: 5000
    // Max: 599
    // Max: 50
    // Max: 30
    // Max: 15
}
```

## Example usage of KHeap

```c++
#include <iostream>
#include "priority_queue/KHeap.h"

int main() {
    std::vector<char> vec { 'b', 'c', 'f', 'a', 'e', 'd' };
    constexpr std::size_t K1 = 4;
    auto min_heap(heap::make_min_k_heap<K1>(std::move(vec)));

    while (!min_heap.empty()) {
        std::cout << "Min: " << min_heap.top() << std::endl;
        min_heap.pop();
    }

    // Output:
    // Min: a
    // Min: b
    // Min: c
    // Min: d
    // Min: e
    // Min: f

    std::cout << std::endl << std::endl;

    constexpr std::size_t K2 = 3;
    auto max_heap(heap::make_max_k_heap<K2, true, std::size_t>());

    max_heap.push(30);
    max_heap.push(15);
    max_heap.push(5000);
    max_heap.push(50);
    max_heap.push(599);

    while (!max_heap.empty()) {
        std::cout << "Max: " << max_heap.top() << std::endl;
        max_heap.pop();
    }

    // Output:
    // Max: 5000
    // Max: 599
    // Max: 50
    // Max: 30
    // Max: 15
}
```

# Priority Queue

The `priority_queue` namespace contains the concrete class [`PriorityQueue`](./PriorityQueue/PriorityQueue.h).
It implements a Priority Queue data-structure based on a generic Heap.
It privately extends either `BinaryHeap` or `KHeap`,
depending on the first template argument received. This allows `PriorityQueue` to access the `protected` vector of nodes stored in `Heap`.
A SFINAE check ensures that `PriorityQueue` can only extend a concrete implementation of `heap::Heap` compatible with the other template arguments received.

The main methods exposed by `PriorityQueue` are:

- `size()`: return the number of elements in the heap. Time complexity: O(1).
- `empty()`: return true iff the heap is empty. Time complexity: O(1).
- `push(const Key& key, const T& element)`: add a new element to the heap and associates the given key to it. Time complexity: O(logN) amortized if using BinaryHeap, O(k*log_k(N)) amortized instead.
- `update_key(const Key& key, const T& element)`: update the key of an existing element in the priority queue. Time: O(logN) amortized if using BinaryHeap, O(k*log_k(N)) amortized instead.
- `contains(const T& element)`: return true iff the given element is in the priority queue. Time: O(1) amortized.
- `top()`: return the top element. Time: O(1).
- `pop()`: remove the top element. Time: O(logN) amortized if using BinaryHeap, O(k*log_k(N)) amortized instead.

**Note**: in order to keep `update_key`'s complexity logarithmic instead of linear, there's a quite important caveat: arbitrary key updates are not allowed.
If you're using a Priority Queue based on a Max-Heap, you can only perform a "increase key" operation.
If you're using a Priority Queue based on a Min-Heap, you can only perform a "decrease key" operation.
Arbitrary key updates may result in undefined behaviour.

A number of utility factory functions are provided to easily create the type of Priority Queue you want, namely:

- `make_min_priority_queue<bool IsAlreadyHeap = false, typename Key, typename Value, typename THash = std::hash<Value>>`: create a Priority Queue based on a Min Heap starting from a vector of values.
- `make_max_priority_queue<bool IsAlreadyHeap = false, typename Key, typename Value, typename THash = std::hash<Value>>`: create a Priority Queue based on a Max Heap starting from a vector of values.
- `make_min_k_priority_queue<std::size_t K, bool IsAlreadyHeap = false, typename Key, typename Value, typename THash = std::hash<Value>>`: create a Priority Queue based on a Min K-ary Heap starting from a vector of keys and a vector of values.
- `make_max_k_priority_queue<std::size_t K, bool IsAlreadyHeap = false, typename Key, typename Value, typename THash = std::hash<Value>>`: create a Priority Queue based on a Max K-Heap starting from a vector of keys and a vector of values.

Each of these functions supports both copy and move semantics for the given inputs.

**Note**: `PriorityQueue` is implemented using `unordered_map` for fast retrieval of keys and element index given an element stored in the underlying `Heap`.
This means that your values' type must have an hash implementation. If you use trivial types (int, double, etc) you're already covered, otherwise you will
have to provide your own custom hash functor as the last template argument of the utility factory functions defined above.

## Example usage of PriorityQueue with BinaryHeap

```c++
#include <iostream>
#include "priority_queue/PriorityQueue.h"

int main() {
    std::vector<std::size_t> keys1{ 5,   4,   1,   3,   6,   0,   2 };
    std::vector<char> vec1{ 'm', 'i', 'n', 'h', 'e', 'a', 'p' };
    auto min_pq1(priority_queue::make_min_priority_queue(keys1, vec1));

    while (!min_pq1.empty()) {
        const auto& [top_key, top_value] = min_pq1.top_key_value();
        std::cout << "Min (key, value): (" << top_key << ", " << top_value << ")" << std::endl;
        min_pq1.pop();
    }

    // Output:
    // Min (key, value): (0, a)
    // Min (key, value): (1, n)
    // Min (key, value): (2, p)
    // Min (key, value): (3, h)
    // Min (key, value): (4, i)
    // Min (key, value): (5, m)
    // Min (key, value): (6, e)

    std::cout << std::endl << std::endl;

    // notice that keys already sorted
    std::vector<std::size_t> keys2{ 0,   2,   4,   6,   8,   10,  12 };
    std::vector<char>        vec2{ 'm', 'i', 'n', 'h', 'e', 'a', 'p' };
    auto min_pq2(priority_queue::make_min_priority_queue<true>(keys2, vec2));

    while (!min_pq2.empty()) {
        const auto& [top_key, top_value] = min_pq2.top_key_value();
        std::cout << "Min (key, value): (" << top_key << ", " << top_value << ")" << std::endl;
        min_pq2.pop();
    }

    // Output:
    // Min (key, value): (0, m)
    // Min (key, value): (2, i)
    // Min (key, value): (4, n)
    // Min (key, value): (6, h)
    // Min (key, value): (8, e)
    // Min (key, value): (10, a)
    // Min (key, value): (12, p)

    std::cout << std::endl << std::endl;
    auto min_pq2_update(priority_queue::make_min_priority_queue<true>(keys2, vec2));

    // Min Heap: keys can only decrease
    min_pq2_update.update_key(5, 'e'); // set key of value 'e' from 8 to 5
    min_pq2_update.update_key(1, 'p');  // set key of value 'p' from 12 to 1

    while (!min_pq2_update.empty()) {
        const auto& [top_key, top_value] = min_pq2_update.top_key_value();
        std::cout << "Min (key, value): (" << top_key << ", " << top_value << ")" << std::endl;
        min_pq2_update.pop();
    }

    // Output:
    // Min (key, value): (0, m)
    // Min (key, value): (1, p)
    // Min (key, value): (2, i)
    // Min (key, value): (4, n)
    // Min (key, value): (5, e)
    // Min (key, value): (6, h)
    // Min (key, value): (10, a)
}
```

## Example usage of PriorityQueue with KHeap

```c++
#include <iostream>
#include "priority_queue/PriorityQueue.h"

int main() {
    std::vector<std::size_t> keys1{ 5,   4,   1,   3,   6,   0,   2 };
    std::vector<char>        vec1{ 'm', 'i', 'n', 'h', 'e', 'a', 'p' };
    constexpr std::size_t K1 = 4;
    auto min_pq1(priority_queue::make_min_k_priority_queue<K1>(keys1, vec1));

    while (!min_pq1.empty()) {
        const auto& [top_key, top_value] = min_pq1.top_key_value();
        std::cout << "Min (key, value): (" << top_key << ", " << top_value << ")" << std::endl;
        min_pq1.pop();
    }

    // Output:
    // Min (key, value): (0, a)
    // Min (key, value): (1, n)
    // Min (key, value): (2, p)
    // Min (key, value): (3, h)
    // Min (key, value): (4, i)
    // Min (key, value): (5, m)
    // Min (key, value): (6, e)

    std::cout << std::endl << std::endl;

    // notice that keys already sorted
    std::vector<std::size_t> keys2{ 0,   2,   4,   6,   8,   10,  12 };
    std::vector<char>        vec2{ 'm', 'i', 'n', 'h', 'e', 'a', 'p' };
    constexpr std::size_t K2 = 3;
    auto min_pq2(priority_queue::make_min_k_priority_queue<K2, true>(keys2, vec2));

    while (!min_pq2.empty()) {
        const auto& [top_key, top_value] = min_pq2.top_key_value();
        std::cout << "Min (key, value): (" << top_key << ", " << top_value << ")" << std::endl;
        min_pq2.pop();
    }

    // Output:
    // Min (key, value): (0, m)
    // Min (key, value): (2, i)
    // Min (key, value): (4, n)
    // Min (key, value): (6, h)
    // Min (key, value): (8, e)
    // Min (key, value): (10, a)
    // Min (key, value): (12, p)

    std::cout << std::endl << std::endl;
    constexpr std::size_t K3 = 5;
    auto min_pq2_update(priority_queue::make_min_k_priority_queue<K3, true>(keys2, vec2));

    // Min Heap: keys can only decrease
    min_pq2_update.update_key(5, 'e'); // set key of value 'e' from 8 to 5
    min_pq2_update.update_key(1, 'p');  // set key of value 'p' from 12 to 1

    while (!min_pq2_update.empty()) {
        const auto& [top_key, top_value] = min_pq2_update.top_key_value();
        std::cout << "Min (key, value): (" << top_key << ", " << top_value << ")" << std::endl;
        min_pq2_update.pop();
    }

    // Output:
    // Min (key, value): (0, m)
    // Min (key, value): (1, p)
    // Min (key, value): (2, i)
    // Min (key, value): (4, n)
    // Min (key, value): (5, e)
    // Min (key, value): (6, h)
    // Min (key, value): (10, a)
}
```

# Best practices adopted

- Most of the methods offered by this library are marked with `noexcept`.
- Every overriden method is marked with `override`.
- This library doesn't perform any heap allocation, at least not directly. Everything is stack-allocated
and built around containers of the standard library (`std::vector` and `std::unordered_map`).
- When possible, if-expressions are performed at compile time.
- Bit-shifting is used instead of division by 2 when possible.
- Heap's methods `heapify_up` and `heapify_down` use iteration instead of recursion.
- Even though the class defined in the `heap` and `priority_queue` namespaces may require a lot of template arguments,
for most practical cases they can be automatically inferred by the compiler. See the examples.

# Test

This project has been developer with Microsoft Visual Studio 2019, but it supports multiplatform builds via [`Bazel`](https://bazel.build/).

The [./tests](./tests) folder contains some unit tests. Coverage isn't 100% as of now, though these libraries have worked well in practice
with more than 90k elements.
To run the [`googletest`](https://github.com/google/googletest) test suite, just execute the following command:

- `bazel test //..`

A note for Windows Users: run that command either in Powershell or in CMD, because it won't work in Git Bash.

# Contributing

Contributions, issues and feature requests are welcome!
The code is short and throughly commented, so you should feel quite comfortable looking at it.
If you have any doubt or suggestion, please open an issue.

Don't be afraid to show your support. Give a ⭐️ if this project helped or inspired you!.