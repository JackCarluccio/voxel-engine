#include "util/IndexableMinHeap.h"

namespace Util {

    IndexableMinHeap::IndexableMinHeap(): size(0), capacity(0) {}

    IndexableMinHeap::IndexableMinHeap(int capacity)
        : size(0), capacity(capacity), pq(capacity), qp(capacity, -1), values(capacity) {}

    // Inserts a key k with associated value into the heap
    void IndexableMinHeap::insert(int k, float value) {
        if (k < 0) {
            throw std::invalid_argument("Key must be non-negative");
        }
        
        ensureCapacity(k);
        
        // If the key already exists, update its value instead
        if (contains(k)) {
            update(k, value);
            return;
        }

        pq[size] = k;
        qp[k] = size;
        values[k] = value;
        swim(size++);
    }

    // Updates the value associated with key k
    void IndexableMinHeap::update(int k, float value) {
        if (k < 0) {
            throw std::invalid_argument("Key must be non-negative");
        } else if (!contains(k)) {
            throw std::invalid_argument("Key does not exist in the heap");
        }

        ensureCapacity(k);

        values[k] = value;
        int index = qp[k];
        swim(index);
        sink(index);
    }

    // Removes the key k from the heap
    bool IndexableMinHeap::remove(int k) {
        int index = qp[k];
        if (index == -1) {
            return false; // Key not found
        }

        removeIndex(index);
        return true;
    }

    // Removes and returns the minimum key from the heap
    int IndexableMinHeap::pop() {
        if (isEmpty()) {
            return -1;
        }
        
        int k = pq[0];
        removeIndex(0);
        return k;
    }

    // Gets the minimum key without removing it from the heap
    int IndexableMinHeap::peek() const noexcept {
        return pq[0];
    }

    // Checks if the heap contains the given key
    bool IndexableMinHeap::contains(int k) const noexcept {
        return qp[k] != -1;
    }

    // Gets the number of elements in the heap
    int IndexableMinHeap::getSize() const noexcept {
        return size;
    }

    // Checks if the heap is empty
    bool IndexableMinHeap::isEmpty() const noexcept {
        return size == 0;
    }

    // Swims an element up the heap to restore heap order
    void IndexableMinHeap::swim(int i) noexcept {
        while (i > 0) {
            int parent = (i - 1) / 2;

            // If the current node is in the correct position, stop
            if (values[pq[i]] >= values[pq[parent]]){
                break;
            }

            swap(i, parent);
            i = parent;
        }
    }

    // Sinks an element down the heap to restore heap order
    void IndexableMinHeap::sink(int i) noexcept {
        while (2 * i + 1 < size) {
            int left = 2 * i + 1; // Left child index

            // Select the smaller child
            if (left + 1 < size && values[pq[left + 1]] < values[pq[left]]) {
                left++;
            }

            // If the current node is in the correct position, stop
            if (values[pq[i]] <= values[pq[left]]) {
                break;
            }

            swap(i, left);
            i = left;
        }
    }

    // Swaps two elements in the heap
    void IndexableMinHeap::swap(int i, int j) noexcept {
        std::swap(pq[i], pq[j]);
        qp[pq[i]] = i;
        qp[pq[j]] = j;
    }

    // Removes the element at index i from the heap
    void IndexableMinHeap::removeIndex(int i) noexcept {
        int k = pq[i];
        swap(i, size - 1);
        qp[k] = -1;
        size--;
        sink(i);
    }

    void IndexableMinHeap::ensureCapacity(int k) noexcept {
        if (k < capacity) {
            return;
        }
        
        capacity = k + 1;
        pq.resize(capacity);
        qp.resize(capacity, -1); // qp has -1 for unused entries
        values.resize(capacity);
    }

}
