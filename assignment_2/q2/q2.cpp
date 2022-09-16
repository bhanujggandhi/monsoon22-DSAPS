#include <iostream>

/*
0-based heap
if i->parent then 2i+1, 2i+2 are children
if i is child then (i-1)/2 is parent
*/
struct Pair {
    int value;
    int ind;
};

class MaxHeap {
   private:
    Pair *arr;
    int size;
    int max_size;

    void swap(Pair *a, Pair *b) {
        Pair c = *a;
        *a = *b;
        *b = c;
    }

   public:
    MaxHeap(int max_ele) {
        max_size = max_ele;
        size = 0;
        arr = new Pair[max_size];
    }

    void insert(int key, int i) {
        if (size == max_size) return;

        arr[size].value = key;
        arr[size].ind = i;
        int ind = size;
        size++;

        while (ind != 0) {
            if (arr[ind].value > arr[(ind - 1) / 2].value) {
                swap(&arr[ind], &arr[(ind - 1) / 2]);
                ind = (ind - 1) / 2;
            } else {
                break;
            }
        }
    }

    Pair remove() {
        if (size <= 0) return {INT32_MIN, INT32_MIN};

        if (size == 1) {
            size = 0;
            return arr[size];
        }

        int maxval = arr[0].value;
        int maxind = arr[0].ind;
        // replace root by last element
        swap(&arr[0], &arr[size - 1]);
        size--;

        int ind = 0;

        while ((2 * ind + 1) < size and (2 * ind + 2) < size) {
            int leftchild = 2 * ind + 1;
            int rightchild = 2 * ind + 2;

            int largechild;

            if (arr[leftchild].value > arr[ind].value and
                arr[leftchild].value >= arr[rightchild].value) {
                largechild = leftchild;
            } else if (arr[rightchild].value > arr[ind].value and
                       arr[rightchild].value > arr[leftchild].value) {
                largechild = rightchild;
            } else {
                break;
            }

            swap(&arr[ind], &arr[largechild]);

            ind = largechild;
        }

        return {maxval, maxind};
    }

    int top() {
        if (size == 0) {
            std::cout << "No top" << std::endl;
            return INT32_MIN;
        }
        return arr[0].value;
    }
};

int main() {
    MaxHeap pq(6);

    pq.insert(3, 1);
    pq.insert(2, 2);
    pq.insert(15, 3);
    std::cout << pq.top() << " ";
    pq.remove();

    std::cout << pq.top() << " ";
    pq.remove();

    pq.insert(5, 1);
    pq.insert(4, 2);
    pq.insert(45, 3);

    std::cout << pq.top() << " ";
    pq.remove();

    std::cout << pq.top() << " ";
    pq.remove();

    std::cout << pq.top() << " ";
    pq.remove();

    std::cout << pq.top() << " ";
    pq.remove();

    pq.top();
    pq.remove();

    return 0;
}