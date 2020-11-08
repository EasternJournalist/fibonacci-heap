
#include <iostream>
#include <fibonacci_heap.h>
#include <vector>
#include <queue>
#include <algorithm>
#include <list>

struct ty {
    int a;
};
int main()
{
    std::fibonacci_heap<int> heap = { 1, 2, 3 };
    heap.insert(3);
    heap.insert(2);
    heap.insert(8);
    heap.insert(6);
    std::fibonacci_heap<int> heap2;
    heap2.insert(9);
    heap2.insert(-1);
    heap2.insert(0);
    auto it = heap2.insert(7);
    heap2.insert(3);
    heap2.insert(8);
    
    heap.merge(heap2);
    heap.modify_key(it, -5);
    while (!heap.empty()) {
        std::cout << heap.top() << " ";
        heap.pop();
    }
    return 0;
}
