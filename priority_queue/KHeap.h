#ifndef K_HEAP_H
#define K_HEAP_H

#include <type_traits>  // std::enable_if
#include <vector>       // std::vector

#include "Heap.h"

namespace heap {

    /**
     * Generic K-ary Heap implementation. Depending on the given comparator function, it can
     * either be a Min K-ary Heap or a Max K-ary Heap.
     * This class shouldn't be invoked directly.
     * Instead, the factories make_min_k_heap and make_max_k_heap should be used.
     *
     * K: the Heap arity. It must be bigger than 2.
     * T: the type of the objects stored in the heap.
     * IsAlreadyHeap: constexpr bool flag. If true, build_heap() won't be called
     *                the class is constructed.
     *                If false, the given vector is considered an already valid heap.
     */
    template <std::size_t K, typename T, bool IsAlreadyHeap = false,
              typename = typename std::enable_if<(K > 2)>::type>
    class KHeap : public Heap<T, IsAlreadyHeap> {
        using super = Heap<T, IsAlreadyHeap>;

        // returns the j-th child of the i-th node.
        [[nodiscard]] static size_t child(const size_t i, const size_t j) noexcept {
            return (K * i) + j + 1;
        }

        // returns true iff the i-th is a leaf
        [[nodiscard]] bool is_leaf(const size_t i) const noexcept {
            return i > ((this->size() - 2) / K);
        }

    protected:
        using compare_t = typename super::compare_t;

        // protected constructor to let the subclass set the comparator after the initialization
        explicit KHeap(const std::vector<T>& inputs) noexcept : super(inputs) {
        }

        // protected constructor to let the subclass set the comparator after the initialization
        explicit KHeap(std::vector<T>&& inputs) noexcept : super(std::move(inputs)) {
        }

        // return the parent of nodes[i]
        [[nodiscard]] std::size_t parent(const std::size_t i) const noexcept final override {
            return (i - 1) / K;
        }

        // starting from a Heap with a misplaced node at the given index,
        // recover the shape and heap properties bubbling the node down.
        void heapify_down(const std::size_t index_to_fix) noexcept override {
            const std::size_t len = this->size();
            std::size_t i = index_to_fix;

            // comp_est is the biggest element in a Max Heap,
            // or the smallest element in a Min Heap
            std::size_t comp_est = i;

            while (!is_leaf(i)) {
                // Time: O(K)
                for (std::size_t j = 0; j < K; ++j) {
                    const auto son = child(i, j);
                    if (son < len && this->comp(this->nodes.at(comp_est), this->nodes.at(son))) {
                        comp_est = son;
                    }
                }

                if (i == comp_est) {
                    return;
                }

                this->swap_nodes(i, comp_est);
                i = comp_est;
            }
        }

        // transform the internal vector in a heap in O(n)
        void build_heap() noexcept override final {
            for (std::size_t i = (this->size() / K) + 1; i > 0; --i) {
                heapify_down(i - 1);
            }
        }

    public:
        // disable public default constructor
        KHeap() = delete;

        explicit KHeap(const std::vector<T>& inputs, compare_t&& comp) noexcept :
            super(inputs, std::move(comp)) {
            this->init();
        }

        explicit KHeap(std::vector<T>&& inputs, compare_t&& comp) noexcept :
            super(std::move(inputs), std::move(comp)) {
            this->init();
        }

        virtual ~KHeap() = default;
    };

    // create a Min K-Heap copying the input vector
    template <std::size_t K, bool IsAlreadyHeap = false, typename T>
    auto make_min_k_heap(const std::vector<T>& inputs) {
        return KHeap<K, T, IsAlreadyHeap>(inputs, std::greater<>{});
    }

    // create a Min K-Heap moving the input vector
    template <std::size_t K, bool IsAlreadyHeap = false, typename T>
    auto make_min_k_heap(std::vector<T>&& inputs = {}) {
        return KHeap<K, T, IsAlreadyHeap>(std::move(inputs), std::greater<>{});
    }

    // create a Max K-Heap copying the input vector
    template <std::size_t K, bool IsAlreadyHeap = false, typename T>
    auto make_max_k_heap(const std::vector<T>& inputs) {
        return KHeap<K, T, IsAlreadyHeap>(inputs, std::less<>{});
    }

    // create a Max K-Heap moving the input vector
    template <std::size_t K, bool IsAlreadyHeap = false, typename T>
    auto make_max_k_heap(std::vector<T>&& inputs = {}) {
        return KHeap<K, T, IsAlreadyHeap>(std::move(inputs), std::less<>{});
    }

}  // namespace heap

#endif  // K_HEAP_H