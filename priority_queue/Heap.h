#ifndef HEAP_H
#define HEAP_H

#include <algorithm>   // std::swap
#include <cassert>     // std::assert
#include <functional>  // std::function
#include <vector>      // std::vector

namespace heap {

    /**
     * Generic Heap abstract class. The elements are allocated in a std::vector<T> container.
     *
     * T: the type of the objects stored in the heap.
     * IsAlreadyHeap: constexpr bool flag. If true, build_heap() won't be called
     *                after the child class is constructed.
     *                If false, the given vector is considered an already valid heap.
     */
    template <typename T, bool IsAlreadyHeap = false>
    class Heap {
    protected:
        // comparison function alias type
        using compare_t = std::function<bool(T, T)>;

        // keep the values in the heap
        std::vector<T> nodes;

        // comparison functor
        // std::greater<> -> Min Heap
        // std::less<>    -> Max Heap
        compare_t comp;

        // protected constructor to let the subclass set the comparator after the initialization
        explicit Heap(const std::vector<T>& inputs) noexcept : nodes(inputs) {
        }

        // protected constructor to let the subclass set the comparator after the initialization
        explicit Heap(std::vector<T>&& inputs) noexcept : nodes(inputs) {
        }

        // return the parent of nodes[i]
        [[nodiscard]] virtual std::size_t parent(std::size_t i) const noexcept = 0;

        // transform the internal vector in a heap in O(n)
        virtual void build_heap() noexcept = 0;

        // starting from a Heap with a misplaced node at the given index,
        // recover the shape and heap properties bubbling the node down.
        virtual void heapify_down(std::size_t index_to_fix) noexcept = 0;

        // swap 2 nodes in the heap
        virtual void swap_nodes(std::size_t i, std::size_t j) noexcept {
            std::swap(nodes.at(i), nodes.at(j));
        }

        // starting from a Heap with a misplaced node at the given index,
        // recover the shape and heap properties in O(logN) bubbling the node up
        void heapify_up(const std::size_t index_to_fix) noexcept {
            std::size_t i = index_to_fix;

            while (i > 0 && comp(nodes.at(parent(i)), nodes.at(i))) {
                const auto p = parent(i);
                swap_nodes(i, p);
                i = p;
            }
        }

        // spend O(n) to build the heap only if the given vector wasn't already a
        // valid heap from the beginning
        void init() noexcept {
            if constexpr (!IsAlreadyHeap) {
                build_heap();
            }
        }

    public:
        // disable public default constructor
        Heap() = delete;

        explicit Heap(const std::vector<T>& inputs, compare_t&& comp) noexcept :
            nodes(inputs), comp(comp) {
        }

        explicit Heap(std::vector<T>&& inputs, compare_t&& comp) noexcept :
            nodes(inputs), comp(comp) {
        }

        // virtual destructor because this is a base class
        virtual ~Heap() = default;

        // return the number of elements in the heap
        [[nodiscard]] size_t size() const noexcept {
            return nodes.size();
        }

        // return true iff the heap is empty
        [[nodiscard]] bool empty() const noexcept {
            return size() == 0;
        }

        // return ths top element of the heap (which must exist)
        [[nodiscard]] const T& top() const {
            assert(size() > 0);

            return nodes.at(0);
        }

        // remove the top element of the heap (which must exist)
        void pop() {
            assert(size() > 0);

            // replace root of the heap with the last element of the vector
            nodes.at(0) = nodes.back();

            // remove the last element of the vector
            nodes.pop_back();

            // the root violates the heap property, so it must be fixed
            heapify_down(0);
        }

        // add a new element to the heap
        template <class... Args>
        void push(Args&&... args) {
            const size_t index_to_fix = size();

            // insert new node at the end of the vector
            nodes.emplace_back(std::forward<Args>(args)...);

            // the root violates the heap property. Let's fix that
            heapify_up(index_to_fix);
        }
    };

}  // namespace heap

#endif  // HEAP_H