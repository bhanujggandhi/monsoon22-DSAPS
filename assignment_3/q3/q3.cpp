#include <iostream>

using namespace std;

struct Node {
    Node* dictionary[26];
    bool isWord;
    int ind;
};

void buildTrie(Node* root, string words[], int n) {
    for (int i = 0; i < n; i++) {
        Node* curr = root;
        for (int j = 0; j < words[i].length(); j++) {
            if (curr->dictionary[words[i][j] - 'a'] == NULL) {
                curr->dictionary[words[i][j] - 'a'] = new Node();
            }
            curr = curr->dictionary[words[i][j] - 'a'];
        }
        curr->ind = i;
        curr->isWord = true;
    }
}

void findwords(char** matrix, Node* root, int i, int j, int r, int c,
               string words[], int X, bool** visited) {

    if (i < 0 or j < 0 or i >= r or j >= c or
        root->dictionary[matrix[i][j] - 'a'] == NULL or visited[i][j] == true)
        return;

    char currchar = matrix[i][j];
    Node* curr = root->dictionary[currchar - 'a'];

    if (curr->isWord == true) {
        cout << words[curr->ind] << endl;
    }

    visited[i][j] = true;
    findwords(matrix, curr, i + 1, j, r, c, words, X, visited);
    findwords(matrix, curr, i - 1, j, r, c, words, X, visited);
    findwords(matrix, curr, i, j - 1, r, c, words, X, visited);
    findwords(matrix, curr, i, j + 1, r, c, words, X, visited);
    visited[i][j] = false;
}

void solve(char** matrix, Node* root, int r, int c, int X, string words[]) {
    bool** visited;
    visited = new bool*[r];
    for (int i = 0; i < r; i++) {
        visited[i] = new bool[c];
        for (int j = 0; j < c; j++) {
            visited[i][j] = false;
        }
    }

    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            findwords(matrix, root, i, j, r, c, words, X, visited);
        }
    }
}

int main() {
    int r, c;
    cin >> r >> c;
    char** matrix;
    matrix = new char*[r];
    for (int i = 0; i < r; i++) {
        matrix[i] = new char[c];
        for (int j = 0; j < c; j++) {
            cin >> matrix[i][j];
        }
    }

    int X;
    cin >> X;
    string words[X];
    for (int i = 0; i < X; i++) {
        cin >> words[i];
    }

    Node* root = new Node();
    buildTrie(root, words, X);
    solve(matrix, root, r, c, X, words);
    return 0;
}

/*
* Remove Duplicated
-> Delete Node when traversed
*/

/*

4 4
o a a n
e t a e
i h k r
i f l v
6
oath
pea
eat
rain
hklf
hf

*/