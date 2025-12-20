#pragma once

#include <vector>
#include <stdexcept>

namespace Util {

    class IndexableMinHeap {
    public:
        IndexableMinHeap();
        IndexableMinHeap(int maxN);

        void insert(int k, float value);
        void update(int k, float value);
        bool remove(int k);

        int pop();
        int peek() const noexcept;

        bool contains(int k) const noexcept;
        bool isEmpty() const noexcept;
        int getSize() const noexcept;
    
    private:
        int size;
        int capacity;
        std::vector<int> pq;
        std::vector<int> qp;
        std::vector<float> values;

        void swim(int i) noexcept;
        void sink(int i) noexcept;
        void swap(int i, int j) noexcept;

        void removeIndex(int i) noexcept;
        void ensureCapacity(int k) noexcept;
    };

}
