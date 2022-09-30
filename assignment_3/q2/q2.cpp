#include <stdio.h>
#include <string.h>
#include <time.h>
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>

#define CHUNK_SIZE 1e+8

using namespace std;

/*
    -> the primary goal of an external sorting algorithm is to minimize the
   number of times information must be read from or written to disk

   ->Under good conditions, reading from a file in sequential order is more
   efficient than reading blocks in random order.

*/

struct Pair {
    long long value;
    long long ind;
};

class MinHeap {
   private:
    vector<Pair> arr;

    void swap(Pair *a, Pair *b) {
        Pair c = (*a);
        *a = (*b);
        *b = (c);
    }

   public:
    void insert(long long key, long long _ind) {

        arr.push_back({key, _ind});

        long long ind = arr.size() - 1;

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
        if (arr.empty()) return {INT64_MIN, INT64_MIN};

        if (arr.size() == 1) {
            Pair ans = {arr[0].value, arr[0].ind};
            arr.pop_back();
            return ans;
        }

        long long maxval = arr[0].value;
        long long maxind = arr[0].ind;
        swap(&arr[0], &arr[arr.size() - 1]);
        arr.pop_back();

        long long ind = 0;

        while ((2 * ind + 1) < arr.size() and (2 * ind + 2) < arr.size()) {
            long long leftchild = 2 * ind + 1;
            long long rightchild = 2 * ind + 2;

            long long smallchild;

            if (arr[leftchild].value < arr[ind].value and
                arr[leftchild].value <= arr[rightchild].value) {
                smallchild = leftchild;
            } else if (arr[rightchild].value < arr[ind].value and
                       arr[rightchild].value <= arr[leftchild].value) {
                smallchild = rightchild;
            } else {
                break;
            }

            swap(&arr[ind], &arr[smallchild]);

            ind = smallchild;
        }

        if (arr.size() == 2) {
            if (arr[ind].value > arr[ind + 1].value)
                swap(&arr[ind], &arr[ind + 1]);
        }

        return {maxval, maxind};
    }

    bool empty() { return arr.empty(); }
};

long long dividefiles(string inputfilename) {
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

    ifstream inputfile(inputfilename, std::ios_base::in);

    for (ind = 0; inputfile.peek() != EOF; ind++) {

        vector<long long> buff;

        long long a;

        for (long long i = 0; i < CHUNK_SIZE; i++) {
            if (inputfile >> a) {
                buff.push_back(a);
            } else {
                break;
            }
        }

        // Conquer
        sort(buff.begin(), buff.end());
        string chunkfilename = "temp" + to_string(ind) + ".txt";
        ofstream chunkfile{chunkfilename};

        for (auto x : buff) {
            chunkfile << x << '\n';
        }

        chunkfile.close();
    }
    inputfile.close();

    return ind;
}

// Combine
void mergefiles(long long totalfiles, string outfilename) {
    vector<ifstream> filesarr;
    vector<bool> finish(totalfiles, false);

    for (long long i = 0; i < totalfiles; i++) {
        string chunkfilename = "temp" + to_string(i) + ".txt";
        filesarr.push_back(ifstream(chunkfilename, std::ios_base::in));
    }

    MinHeap minh;

    for (long long i = 0; i < totalfiles; i++) {
        long long curr;
        if (!finish[i] and filesarr[i] >> curr) {
            minh.insert(curr, i);
        } else {
            filesarr[i].close();
            finish[i] = true;
        }
    }

    ofstream outfile{outfilename};

    while (!minh.empty()) {
        Pair top = minh.remove();
        outfile << top.value << "\n";
        long long i = top.ind;
        long long curr;
        if (!finish[i] and filesarr[i] >> curr) {
            minh.insert(curr, i);
        } else {
            filesarr[i].close();
            finish[i] = true;
        }
    }
    outfile.close();
}

void cleanfiles(long long totalfiles) {
    for (long long i = 0; i < totalfiles; i++) {
        string chunkfilename = "temp" + to_string(i) + ".txt";
        remove(chunkfilename.c_str());
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "Please enter two arguments only" << endl;
        return 1;
    }

    auto start = clock();

    string inputfilename(argv[1]);
    string outputfilename(argv[2]);
    long long totalfiles = dividefiles(inputfilename);

    cout << "Total Number of Integers in a File " << CHUNK_SIZE << endl;

    cout << "Total Number of Files: " << totalfiles << endl;
    mergefiles(totalfiles, outputfilename);

    auto end = clock();

    cleanfiles(totalfiles);
    double totaltime = (double)(end - start) / CLOCKS_PER_SEC;
    cout << totaltime << "s" << endl;

    return 0;
}