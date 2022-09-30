#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <fstream>
#include <iostream>

#define CHUNK_SIZE 1024000

using namespace std;

/*
    -> the primary goal of an external sorting algorithm is to minimize the
   number of times information must be read from or written to disk

   ->Under good conditions, reading from a file in sequential order is more
   efficient than reading blocks in random order.

*/

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

// void mergefiles() { string chunkfilename = "temp" + to_string(ind) + ".txt";
// }

int main(int argc, char* argv[]) {
    // if (argc != 3) {
    //     cout << "Please enter two arguments only" << endl;
    //     return 1;
    // }
    string inp = "input.txt";
    long long totalfiles = divide(inp);

    cout << totalfiles << endl;

    return 0;
}