#include <algorithm>
#include <iostream>

typedef long long ll;

/*
0-based heap
if i->parent then 2i+1, 2i+2 are children
if i is child then (i-1)/2 is parent
*/

void merge(ll arr[], int l, int m, int r) {
    int len1 = m - l + 1;
    int len2 = r - m;

    ll *first = new ll[len1];
    ll *second = new ll[len2];

    int mainArrayIndex = l;
    for (int i = 0; i < len1; i++) first[i] = arr[mainArrayIndex++];

    mainArrayIndex = m + 1;
    for (int i = 0; i < len2; i++) second[i] = arr[mainArrayIndex++];

    int index1 = 0;
    int index2 = 0;
    mainArrayIndex = l;

    while (index1 < len1 && index2 < len2) {

        if (first[index1] < second[index2])
            arr[mainArrayIndex++] = first[index1++];
        else
            arr[mainArrayIndex++] = second[index2++];
    }
    while (index1 < len1) arr[mainArrayIndex++] = first[index1++];
    while (index2 < len2) arr[mainArrayIndex++] = second[index2++];

    delete[] first;
    delete[] second;
}

void mergeSort(ll arr[], int l, int r) {
    if (l >= r) return;

    int mid = l + (r - l) / 2;
    mergeSort(arr, l, mid);
    mergeSort(arr, mid + 1, r);

    merge(arr, l, mid, r);
}

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

// Heap sort function
void abssort(ll arr[], int n) {
    MaxHeap maxhp(n);
    for (int i = 0; i < n; i++) {
        maxhp.insert(abs(arr[i]), i);
    }

    for (int i = n - 1; i >= 0; i--) {
        arr[i] = maxhp.top().value;
        maxhp.remove();
    }
}

void solve(ll popularity[], int n, int k) {
    // 1. Calculate Max Sum of all positives
    ll maxsum = 0;
    for (int i = 0; i < n; i++)
        if (popularity[i] > 0) maxsum += popularity[i];

    if (k == 1) {
        std ::cout << maxsum << std::endl;
        return;
    };

    ll ansarr[k];
    ll ansind = 0;

    // 2. Sort the array
    abssort(popularity, n);

    // 3. Heap of Pair
    MaxHeap hp(k);
    // std::cout << maxsum << " ";
    ansarr[ansind++] = maxsum;
    hp.insert(maxsum - abs(popularity[0]), 0);
    int i = 1;
    while (i != k) {
        ll topval = hp.top().value;
        int topind = hp.top().ind;

        // std::cout << topval << " ";
        ansarr[ansind++] = topval;
        i++;

        hp.remove();
        if (topind + 1 < n) {
            hp.insert(
                topval + abs(popularity[topind]) - abs(popularity[topind + 1]),
                topind + 1);
            hp.insert(topval - abs(popularity[topind + 1]), topind + 1);
        }
    }
    mergeSort(ansarr, 0, k);
    std::reverse(ansarr, ansarr + k);
    for (int i = 0; i < k; i++) {
        std::cout << ansarr[i] << " ";
    }
}

// Driver Code
int main() {
    int n, k;
    std::cin >> n >> k;

    ll popularity[n];

    for (int i = 0; i < n; i++) std::cin >> popularity[i];

    solve(popularity, n, k);

    return 0;
}