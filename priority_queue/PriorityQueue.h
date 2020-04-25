#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <algorithm>      // std::swap, std::find
#include <cassert>        // std::assert
#include <functional>     // std::function
#include <type_traits>    // std::enable_if
#include <unordered_map>  // std::unordered_map

#include "BinaryHeap.h"
#include "Heap.h"
#include "KHeap.h"

namespace priority_queue {

    // utility enum class used in PriorityQueue.update_key() to understand at compile time
    // whether PriorityQueue is using a Min Heap or a Max Heap implementation.
    enum class Type { min_heap, max_heap };

    /**
     * Generic Priority Queue based on a Heap. Whether it is based on a MinHeap or a MaxHeap
     * depends on CompareFactory object used for initialization.
     * This class shouldn't be invoked directly.
     * Instead, the factories make_min_priority_queue, make_max_priority_queue,
     * make_min_k_priority_queue, make_max_k_priority_queue should be used.
     *
     * Heap: type of the parent class of PriorityQueue. It must derive heap::Heap.
     * Key: type of the keys used to order the Heap.
     * T: type of the elements stored in the Heap underlying the Priority Queue.
     * IsAlreadyHeap: constexpr flag. If true, build_heap() won't be called after the class is
     *                constructed. If false, the given input is considered an already valid heap
     *                according to its keys.
     * T_Hash: functor used to hash values of type T. Necessary only if T is a non-trivial type.
     * HeapType: enum utility used to understand whether Heap is a Min Heap or a Max Heap.
     */
    template <class Heap, typename Key, typename T, bool IsAlreadyHeap = false,
              typename THash = std::hash<T>, Type HeapType = Type::min_heap,
              typename = std::enable_if<std::is_base_of<heap::Heap<T, IsAlreadyHeap>, Heap>::value>>
    class PriorityQueue : Heap {
        using super = Heap;
        using key_map_type = std::unordered_map<T, Key, THash>;
        using index_map_type = std::unordered_map<T, std::size_t, THash>;
        using compare_t = std::function<bool(T, T)>;
        using compare_factory_t = std::function<compare_t(key_map_type&)>;

        // keep track of the value of the keys of each node.
        // key_map[element] -> key assigned to element
        key_map_type key_map;

        // keep track of the index of the nodes stored in the Heap.
        // index_map[element] -> index in this->nodes of element
        index_map_type index_map;

        // initialize key_map
        template <typename UKey, typename U, typename UHash>
        [[nodiscard]] static auto build_key_map(const std::vector<UKey>& keys,
                                                const std::vector<U>& node_list) {
            assert(keys.size() == node_list.size());

            std::unordered_map<U, UKey, UHash> local_key_map(node_list.size());

            // traverses keys and node_list at the same time
            std::size_t index = 0;
            for (const auto& node : node_list) {
                local_key_map[node] = keys[index];
                index++;
            }

            return local_key_map;
        }

        // initialize index_map
        template <typename U, typename UHash>
        [[nodiscard]] static auto build_index_map(const std::vector<U>& node_list) {
            std::unordered_map<U, std::size_t, UHash> local_index_map(node_list.size());

            std::size_t index = 0;
            for (const auto& node : node_list) {
                local_index_map[node] = index;
                index++;
            }

            return local_index_map;
        }

        // initialize class
        void init(compare_factory_t&& comp_factory) {
            // set comparator in base class after initialization of key_map and index_map.
            // We can't set it directly in super(inputs, comp_factory(key_map) because we need
            // to initialize key_map first, and member objects can only be initialized after
            // the parent class is completely initialized.
            this->comp = comp_factory(key_map);
            super::init();
        }

    protected:
        void swap_nodes(std::size_t i, std::size_t j) noexcept {
            auto node_i = this->nodes.at(i);
            auto node_j = this->nodes.at(j);

            // swap the indexes
            std::swap(index_map.at(node_i), index_map.at(node_j));

            // swap the nodes
            super::swap_nodes(i, j);
        }

    public:
        PriorityQueue() = delete;

        explicit PriorityQueue(const std::vector<Key>& keys, const std::vector<T>& inputs,
                               compare_factory_t&& comp_factory) :
            super(inputs),
            key_map(build_key_map<Key, T, THash>(keys, this->nodes)),
            index_map(build_index_map<T, THash>(this->nodes)) {
            init(std::move(comp_factory));
        }

        explicit PriorityQueue(std::vector<Key>&& keys, std::vector<T>&& inputs,
                               compare_factory_t&& comp_factory) :
            super(std::move(inputs)),
            key_map(build_key_map<Key, T, THash>(std::move(keys), this->nodes)),
            index_map(build_index_map<T, THash>(this->nodes)) {
            init(std::move(comp_factory));
        }

        ~PriorityQueue() = default;

        // return the number of elements in the heap.
        // Time: O(1).
        // Space: O(1).
        [[nodiscard]] std::size_t size() const noexcept {
            return super::size();
        }

        // return true iff the heap is empty.
        // Time: O(1).
        // Space: O(1).
        [[nodiscard]] bool empty() const noexcept {
            return super::empty();
        }

        // add a new element to the heap and associates the given key to it.
        // Time: O(logN) amortized if using BinaryHeap, O(k*log_k(N)) amortized instead.
        // Space: O(1) amortized.
        void push(const Key& key, const T& element) {
            const std::size_t index_to_fix = this->size();
            index_map[element] = index_to_fix;
            key_map[element] = key;

            super::push(element);
        }

        // update the key of an element in the priority queue.
        // The element must exist in the priority queue.
        // Time: O(logN) amortized if using BinaryHeap, O(k*log_k(N)) amortized instead.
        // Space: O(1) amortized.
        void update_key(const Key& key, const T& element) {
            const std::size_t index_to_fix = index_map.at(element);
            key_map.at(element) = key;

            // recover the heap property
            if constexpr (HeapType == Type::min_heap) {
                // we're using a MinHeap, thus we are decreasing the key
                this->heapify_up(index_to_fix);
            } else {
                // we're using a MaxHeap, thus we are increasing the key
                this->heapify_down(index_to_fix);
            }
        }

        // return the value of the key of the given element.
        // Time: O(1) amortized.
        // Space: O(1).
        [[nodiscard]] const Key& key_at(const T& element) const {
            return key_map.at(element);
        }

        // return true iff the given element is in the priority queue.
        // Time: O(1) amortized.
        // Space: O(1).
        [[nodiscard]] bool contains(const T& element) const noexcept {
            const auto search = index_map.find(element);
            return search != index_map.end();
        }

        // return the top element.
        // Time: O(1).
        // Space: O(1).
        [[nodiscard]] const T& top() const {
            return super::top();
        }

        // return top key-value pair.
        // Time: O(1).
        // Space: O(1).
        [[nodiscard]] std::pair<const Key&, const T&> top_key_value() const {
            const auto& top_value = super::top();
            const auto& top_key = key_at(top_value);
            return {top_key, top_value};
        }

        // remove the top element.
        // Time: O(logN) amortized if using BinaryHeap, O(k*log_k(N)) amortized instead.
        // Space: O(1).
        void pop() {
            assert(this->size() > 0);

            const std::size_t index_to_remove = 0;
            const auto node = this->nodes.at(index_to_remove);
            index_map.erase(node);
            key_map.erase(node);

            // replace root of the heap with the last element of the vector
            this->nodes.at(0) = this->nodes.back();

            // remove the last element of the vector
            this->nodes.pop_back();

            // no need to recover the heap property if the heap is empty
            if (this->size() > 0) {
                const auto front = this->nodes.front();
                index_map.at(front) = 0;

                // the root violates the heap property. Let's fix that
                this->heapify_down(0);
            }
        }
    };

    namespace detail {

        inline auto min_heap_comp_factory = [](auto& key_map) {
            return [&key_map](auto&& a, auto&& b) -> bool {
                return key_map.at(a) > key_map.at(b);
            };
        };

        inline auto max_heap_comp_factory = [](auto& key_map) {
            return [&key_map](auto&& a, auto&& b) -> bool {
                return key_map.at(a) < key_map.at(b);
            };
        };

    }  // namespace detail

    // create a Priority Queue based on a Min Heap. It copies the given vectors.
    template <bool IsAlreadyHeap = false, typename Key, typename Value,
              typename THash = std::hash<Value>>
    auto make_min_priority_queue(const std::vector<Key>& keys, const std::vector<Value>& inputs) {
        using pq = PriorityQueue<heap::BinaryHeap<Value, IsAlreadyHeap>, Key, Value, IsAlreadyHeap,
                                 THash, Type::min_heap>;
        return pq(keys, inputs, detail::min_heap_comp_factory);
    }

    // create a Priority Queue based on a Min Heap. It moves the given vectors.
    template <bool IsAlreadyHeap = false, typename Key, typename Value,
              typename THash = std::hash<Value>>
    auto make_min_priority_queue(std::vector<Key>&& keys = {}, std::vector<Value>&& inputs = {}) {
        using pq = PriorityQueue<heap::BinaryHeap<Value, IsAlreadyHeap>, Key, Value, IsAlreadyHeap,
                                 THash, Type::min_heap>;
        return pq(std::move(keys), std::move(inputs), detail::min_heap_comp_factory);
    }

    // create a Priority Queue based on a Max Heap. It copies the given vectors.
    template <bool IsAlreadyHeap = false, typename Key, typename Value,
              typename THash = std::hash<Value>>
    auto make_max_priority_queue(const std::vector<Key>& keys, const std::vector<Value>& inputs) {
        using pq = PriorityQueue<heap::BinaryHeap<Value, IsAlreadyHeap>, Key, Value, IsAlreadyHeap,
                                 THash, Type::max_heap>;
        return pq(keys, inputs, detail::max_heap_comp_factory);
    }

    // create a Priority Queue based on a Max Heap. It moves the given vectors.
    template <bool IsAlreadyHeap = false, typename Key, typename Value,
              typename THash = std::hash<Value>>
    auto make_max_priority_queue(std::vector<Key>&& keys = {}, std::vector<Value>&& inputs = {}) {
        using pq = PriorityQueue<heap::BinaryHeap<Value, IsAlreadyHeap>, Key, Value, IsAlreadyHeap,
                                 THash, Type::max_heap>;
        return pq(std::move(keys), std::move(inputs), detail::max_heap_comp_factory);
    }

    // create a Priority Queue based on a Min K-Heap. It copies the given vectors.
    template <std::size_t K, bool IsAlreadyHeap = false, typename Key, typename Value,
              typename THash = std::hash<Value>>
    auto make_min_k_priority_queue(const std::vector<Key>& keys, const std::vector<Value>& inputs) {
        using pq = PriorityQueue<heap::KHeap<K, Value, IsAlreadyHeap>, Key, Value, IsAlreadyHeap,
                                 THash, Type::min_heap>;
        return pq(keys, inputs, detail::min_heap_comp_factory);
    }

    // create a Priority Queue based on a Min K-Heap. It moves the given vectors.
    template <std::size_t K, bool IsAlreadyHeap = false, typename Key, typename Value,
              typename THash = std::hash<Value>>
    auto make_min_k_priority_queue(std::vector<Key>&& keys = {}, std::vector<Value>&& inputs = {}) {
        using pq = PriorityQueue<heap::KHeap<K, Value, IsAlreadyHeap>, Key, Value, IsAlreadyHeap,
                                 THash, Type::min_heap>;
        return pq(std::move(keys), std::move(inputs), detail::min_heap_comp_factory);
    }

    // create a Priority Queue based on a Max K-Heap. It copies the given vectors.
    template <std::size_t K, bool IsAlreadyHeap = false, typename Key, typename Value,
              typename THash = std::hash<Value>>
    auto make_max_k_priority_queue(const std::vector<Key>& keys, const std::vector<Value>& inputs) {
        using pq = PriorityQueue<heap::KHeap<K, Value, IsAlreadyHeap>, Key, Value, IsAlreadyHeap,
                                 THash, Type::max_heap>;
        return pq(keys, inputs, detail::max_heap_comp_factory);
    }

    // create a Priority Queue based on a Max K-Heap. It moves the given vectors.
    template <std::size_t K, bool IsAlreadyHeap = false, typename Key, typename Value,
              typename THash = std::hash<Value>>
    auto make_max_k_priority_queue(std::vector<Key>&& keys = {}, std::vector<Value>&& inputs = {}) {
        using pq = PriorityQueue<heap::KHeap<K, Value, IsAlreadyHeap>, Key, Value, IsAlreadyHeap,
                                 THash, Type::max_heap>;
        return pq(std::move(keys), std::move(inputs), detail::max_heap_comp_factory);
    }

}  // namespace priority_queue

#endif  // PRIORITY_QUEUE_H