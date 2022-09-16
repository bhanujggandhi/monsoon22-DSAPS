#include <algorithm>
#include <iostream>

typedef long long ll;

/*
0-based heap
if i->parent then 2i+1, 2i+2 are children
if i is child then (i-1)/2 is parent
*/
struct Pair {
    ll value;
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

    void insert(ll key, int i) {
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

        ll maxval = arr[0].value;
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

        if (size == 2) {
            if (arr[ind].value < arr[ind + 1].value)
                swap(&arr[ind], &arr[ind + 1]);
        }

        return {maxval, maxind};
    }

    Pair top() {
        if (size == 0) {
            std::cout << "No top" << std::endl;
            return {INT32_MIN, INT32_MIN};
        }
        return arr[0];
    }
};

bool cmp(int a, int b) { return abs(a) < abs(b); }

void solve(ll popularity[], int n, int k) {
    // 1. Calculate Max Sum of all positives
    ll maxsum = 0;
    for (int i = 0; i < n; i++)
        if (popularity[i] > 0) maxsum += popularity[i];

    if (k == 1) {
        std ::cout << maxsum << std::endl;
        return;
    };

    // 2. Sort the array
    std::sort(popularity, popularity + n, cmp);

    // 3. Heap of Pair
    MaxHeap hp(k);
    std::cout << maxsum << " ";
    hp.insert(maxsum - abs(popularity[0]), 0);
    int i = 1;
    while (i != k) {
        ll topval = hp.top().value;
        int topind = hp.top().ind;

        std::cout << topval << " ";
        i++;

        hp.remove();
        if (topind + 1 < n) {
            hp.insert(
                topval + abs(popularity[topind]) - abs(popularity[topind + 1]),
                topind + 1);
            hp.insert(topval - abs(popularity[topind + 1]), topind + 1);
        }
    }
}

int main() {
    int n, k;
    std::cin >> n >> k;

    ll popularity[n];

    for (int i = 0; i < n; i++) std::cin >> popularity[i];

    solve(popularity, n, k);

    return 0;
}