#include <bits/stdc++.h>

using namespace std;

struct Pair {
    long long value;
    ifstream ind;
};

class MaxHeap {
   private:
    Pair *arr;
    int size;
    int max_size;

    void swap(Pair *a, Pair *b) {
        Pair c = move(*a);
        *a = move(*b);
        *b = move(c);
    }

   public:
    MaxHeap(int max_ele) {
        max_size = max_ele;
        size = 0;
        arr = new Pair[max_size];
    }

    void insert(long long key, ifstream fs) {
        if (size == max_size) return;

        arr[size].value = key;
        arr[size].ind = move(fs);
        int ind = size;
        size++;

        while (ind != 0) {
            if (arr[ind].value < arr[(ind - 1) / 2].value) {
                swap(&arr[ind], &arr[(ind - 1) / 2]);
                ind = (ind - 1) / 2;
            } else {
                break;
            }
        }
    }

    Pair remove() {
        if (size <= 0) return {INT32_MIN};

        if (size == 1) {
            size = 0;
            return {arr[size].value, move(arr[size].ind)};
        }

        long long maxval = arr[0].value;
        ifstream maxfs = move(arr[0].ind);
        // replace root by last element
        swap(&arr[0], &arr[size - 1]);
        size--;

        int ind = 0;

        while ((2 * ind + 1) < size and (2 * ind + 2) < size) {
            int leftchild = 2 * ind + 1;
            int rightchild = 2 * ind + 2;

            int largechild;

            if (arr[leftchild].value < arr[ind].value and
                arr[leftchild].value <= arr[rightchild].value) {
                largechild = leftchild;
            } else if (arr[rightchild].value < arr[ind].value and
                       arr[rightchild].value < arr[leftchild].value) {
                largechild = rightchild;
            } else {
                break;
            }

            swap(&arr[ind], &arr[largechild]);

            ind = largechild;
        }

        if (size == 2) {
            if (arr[ind].value < arr[ind + 1].value)
                swap(&arr[ind], &arr[ind + 1]);
        }

        return {maxval, move(maxfs)};
    }

    Pair top() {
        if (size == 0) {
            std::cout << "No top" << std::endl;
            return {INT32_MIN};
        }
        return move(arr[0]);
    }
};

int main() {
    MaxHeap hp(10);
    ifstream fs("input.txt");
    hp.insert(32, move(fs));
    hp.insert(92, move(fs));
    hp.insert(1000, move(fs));
    hp.insert(313221, move(fs));
    hp.insert(3083198, move(fs));
    hp.insert(3081, move(fs));
    hp.insert(2380, move(fs));

    cout << hp.remove().value << endl;
    cout << hp.remove().value << endl;
    cout << hp.remove().value << endl;
    cout << hp.remove().value << endl;
    cout << hp.remove().value << endl;
    ifstream f = hp.remove().ind;
    long long a;
    f >> a;
    cout << a;
    f >> a;
    cout << a;

    return 0;
}