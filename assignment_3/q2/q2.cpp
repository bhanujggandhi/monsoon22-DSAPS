#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <fstream>
#include <iostream>

// #define CHUNK_SIZE 1024000
#define CHUNK_SIZE 10

using namespace std;

/*
    -> the primary goal of an external sorting algorithm is to minimize the
   number of times information must be read from or written to disk

   ->Under good conditions, reading from a file in sequential order is more
   efficient than reading blocks in random order.

*/

typedef long long ll;

/*
0-based heap
if i->parent then 2i+1, 2i+2 are children
if i is child then (i-1)/2 is parent
*/
struct Pair {
    long long value;
    ifstream ind;
};

class MinHeap {
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
    MinHeap(int max_ele) {
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

        ll maxval = arr[0].value;
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

long long divide(string inputfilename) {
    /*
        1. open input file
        2. Read first k bytes
        3. While not end of the file:
            open new output file
            write k bytes
            close output file
            read next k block
    */

    long long ind = 0;

    fstream inputfile(inputfilename, std::ios_base::in);

    for (ind = 0; inputfile.peek() != EOF; ind++) {

        vector<long long> buff(CHUNK_SIZE);

        long long a;

        for (long long i = 0; i < CHUNK_SIZE; i++) {
            if (inputfile >> a) {
                buff[i] = a;
            } else {
                break;
            }
        }

        // Conquer
        sort(buff.begin(), buff.end());
        string chunkfilename = "temp" + to_string(ind) + ".txt";
        ofstream out_file{chunkfilename};

        for (auto x : buff) {
            out_file << x << " ";
        }

        out_file.close();
    }
    inputfile.close();

    return ind;
}

void mergefiles(long long totalfiles) {
    vector<ifstream> filesarr(totalfiles);
    vector<bool> finish(totalfiles, false);

    for (long long i = 0; i < totalfiles; i++) {
        string chunkfilename = "temp" + to_string(i) + ".txt";
        ifstream file(chunkfilename, std::ios_base::in);
        filesarr[i] = move(file);
    }

    MinHeap minh(totalfiles);

    for (long long i = 0; i < totalfiles; i++) {
        ifstream fs = move(filesarr[i]);
        long long curr;
        if (!finish[i] and fs >> curr) {
            minh.insert(curr, move(fs));
        } else {
            fs.close();
            finish[i] = true;
        }
    }
}

int main(int argc, char *argv[]) {
    // if (argc != 3) {
    //     cout << "Please enter two arguments only" << endl;
    //     return 1;
    // }
    string inp = "input.txt";
    long long totalfiles = divide(inp);

    cout << totalfiles << endl;
    mergefiles(totalfiles);

    return 0;
}