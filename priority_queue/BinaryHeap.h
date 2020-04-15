#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include <vector>  // std::vector

#include "Heap.h"

namespace heap {

    /**
     * Generic Binary Heap implementation. Depending on the given comparator function, it can
     * either be a Min Heap or a Max Heap.
     * This class shouldn't be invoked directly.
     * Instead, the factories make_min_heap and make_max_heap should be used.
     *
     * T: the type of the objects stored in the heap.
     * IsAlreadyHeap: constexpr bool flag. If true, build_heap() won't be called
     *                after the class is constructed.
     *                If false, the given vector is considered an already valid heap.
     */
    template <typename T, bool IsAlreadyHeap = false>
    class BinaryHeap : public Heap<T, IsAlreadyHeap> {
        using super = Heap<T, IsAlreadyHeap>;
        // returns the left child of nodes[i]
        [[nodiscard]] static std::size_t left(const std::size_t i) noexcept {
            return (i << 1) + 1;
        }

        // returns the right child of nodes[i]
        [[nodiscard]] static std::size_t right(const std::size_t i) noexcept {
            return (i << 1) + 2;
        }

        // returns true iff the nodes[i] is a leaf
        [[nodiscard]] bool is_leaf(const size_t i) const noexcept {
            return i >= (this->size() >> 1) + 1;
        }

    protected:
        using compare_t = typename super::compare_t;

        // protected constructor to let the subclass set the comparator after the initialization
        explicit BinaryHeap(const std::vector<T>& inputs) noexcept : super(inputs) {
        }

        // protected constructor to let the subclass set the comparator after the initialization
        explicit BinaryHeap(std::vector<T>&& inputs) noexcept : super(std::move(inputs)) {
        }

        // return the parent of nodes[i]
        [[nodiscard]] std::size_t parent(const std::size_t i) const noexcept override final {
            return (i - 1) >> 1;
        }

        // starting from a Heap with a misplaced node at the given index,
        // recover the shape and heap properties bubbling the node down
        void heapify_down(const std::size_t index_to_fix) noexcept override {
            const std::size_t len = this->size();
            std::size_t i = index_to_fix;

            while (!is_leaf(i)) {
                const auto l = left(i);
                const auto r = right(i);

                // comp_est is the biggest element in a Max Heap,
                // or the smallest element in a Min Heap
                std::size_t comp_est = i;

                if (l < len && this->comp(this->nodes.at(i), this->nodes.at(l))) {
                    comp_est = l;
                }
                if (r < len && this->comp(this->nodes.at(comp_est), this->nodes.at(r))) {
                    comp_est = r;
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
            for (std::size_t i = (this->size() >> 1) + 1; i > 0; --i) {
                heapify_down(i - 1);
            }
        }

    public:
        // disable public default constructor
        BinaryHeap() = delete;

        explicit BinaryHeap(const std::vector<T>& inputs, compare_t&& comp) noexcept :
            super(inputs, std::move(comp)) {
            this->init();
        }

        explicit BinaryHeap(std::vector<T>&& inputs, compare_t&& comp) noexcept :
            super(std::move(inputs), std::move(comp)) {
            this->init();
        }

        virtual ~BinaryHeap() = default;
    };

    // create a Min Heap copying the input vector
    template <bool IsAlreadyHeap = false, typename T>
    auto make_min_heap(const std::vector<T>& inputs) {
        return BinaryHeap<T, IsAlreadyHeap>(inputs, std::greater<>{});
    }

    // create a Min Heap moving the input vector
    template <bool IsAlreadyHeap = false, typename T>
    auto make_min_heap(std::vector<T>&& inputs = {}) {
        return BinaryHeap<T, IsAlreadyHeap>(std::move(inputs), std::greater<>{});
    }

    // create a Max Heap copying the input vector
    template <bool IsAlreadyHeap = false, typename T>
    auto make_max_heap(const std::vector<T>& inputs) {
        return BinaryHeap<T, IsAlreadyHeap>(inputs, std::less<>{});
    }

    // create a Max Heap moving the input vector
    template <bool IsAlreadyHeap = false, typename T>
    auto make_max_heap(std::vector<T>&& inputs = {}) {
        return BinaryHeap<T, IsAlreadyHeap>(std::move(inputs), std::less<>{});
    }

}  // namespace heap

#endif  // BINARY_HEAP_H