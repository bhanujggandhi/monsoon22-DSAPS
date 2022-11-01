#include <iostream>

using namespace std;

/// @brief Structure Definition of the Node of the Trie
struct Node {
    Node* dictionary[26];
    bool isWord;
    int ind;
    bool found;
};

/// @brief Function to build a trie for the array of words
/// @param root
/// @param words
/// @param n
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

/// @brief Function to find words by using Backtracking Technique
/// @param matrix
/// @param root
/// @param i
/// @param j
/// @param r
/// @param c
/// @param words
/// @param X
/// @param visited
void findwords(char** matrix, Node* root, int i, int j, int r, int c,
               string words[], int X, bool** visited, int& count) {

    if (i < 0 or j < 0 or i >= r or j >= c or
        root->dictionary[matrix[i][j] - 'a'] == NULL or visited[i][j] == true)
        return;

    char currchar = matrix[i][j];
    Node* curr = root->dictionary[currchar - 'a'];

    if (curr->isWord == true) {
        // cout << words[curr->ind] << endl;
        count++;
        curr->isWord = false;
        curr->found = true;
    }

    visited[i][j] = true;
    findwords(matrix, curr, i + 1, j, r, c, words, X, visited, count);
    findwords(matrix, curr, i - 1, j, r, c, words, X, visited, count);
    findwords(matrix, curr, i, j - 1, r, c, words, X, visited, count);
    findwords(matrix, curr, i, j + 1, r, c, words, X, visited, count);
    visited[i][j] = false;
}

/// @brief Function to solve the formal problem of finding words in the grid
/// puzzle
/// @param matrix
/// @param root
/// @param r
/// @param c
/// @param X
/// @param words
void solve(char** matrix, Node* root, int r, int c, int X, string words[],
           int& count) {
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
            findwords(matrix, root, i, j, r, c, words, X, visited, count);
        }
    }
}

/// @brief Utility function to get all the words present after a certain Trie
/// Node
/// @param root
/// @param currword
void getWords(Node* root, string currword) {
    if (root->found) cout << currword << " ";

    for (int i = 0; i < 26; i++) {
        if (root->dictionary[i] == NULL) continue;

        currword.push_back(i + 'a');
        getWords(root->dictionary[i], currword);
        currword.pop_back();
    }
}

/// @brief Driver Code
/// @return Status of the Program
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
    int count = 0;
    solve(matrix, root, r, c, X, words, count);
    // cout << count << endl;
    getWords(root, "");
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


4 4
c a x i
y r t t
d r a c
s p e y
1
car

*/