#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <iostream>

#define CHUNK_SIZE 1024000

using namespace std;

FILE* openfile(char* filename, string mode) {
    FILE* curr = fopen(filename, mode.c_str());
    if (curr == NULL) {
        cout << "Could not open the file" << endl;
        return NULL;
    }

    return curr;
}

size_t filesize(char* inputfile) {
    FILE* inp = openfile(inputfile, "rb");
    if (inp) {
        fseek(inp, 0, SEEK_END);
        size_t size = ftell(inp) + 1;
        return size;
    }
    return -1;
}

/*
    -> the primary goal of an external sorting algorithm is to minimize the
   number of times information must be read from or written to disk

   ->Under good conditions, reading from a file in sequential order is more
   efficient than reading blocks in random order.

*/

void divide(char* inputfile) {
    /*
        1. open input file
        2. Read first k bytes
        3. While not end of the file:
            open new output file
            write k bytes
            close output file
            read next k block
    */

    char buff[BUFSIZ];
    long accumulated, i;
    size_t size;
    size_t read;
    size_t written;
    FILE* inp = openfile(inputfile, "r");
    size_t segments = (filesize(inputfile) / CHUNK_SIZE) + 1;
    FILE* chunkf;
    char chunkfile[256];
    if (inp != NULL) {
        for (i = 0; i < segments; i++) {
            accumulated = 0;
            sprintf(chunkfile, "%s%ld.txt", "temp", i);
            chunkf = fopen(chunkfile, "w");
            if (chunkf) {
                while (fgets(buff, BUFSIZ, inp) && accumulated <= CHUNK_SIZE) {
                    accumulated += strlen(buff);
                    // sort(buff, buff + BUFSIZ);
                    fputs(buff, chunkf);
                }
                fclose(chunkf);
            }
        }
        fclose(inp);
    }
}

int main(int argc, char* argv[]) {
    // if (argc != 3) {
    //     cout << "Please enter two arguments only" << endl;
    //     return 1;
    // }

    divide("input.txt");

    return 0;
}