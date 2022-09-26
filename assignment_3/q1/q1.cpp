#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Node {
    Node* dictionary[26];
    bool isWord;
    int ind;
};

void buildTrie(Node* root, vector<string>& dict) {
    int n = dict.size();
    for (int i = 0; i < n; i++) {
        Node* curr = root;
        for (int j = 0; j < dict[i].length(); j++) {
            if (curr->dictionary[dict[i][j] - 'a'] == NULL) {
                curr->dictionary[dict[i][j] - 'a'] = new Node();
            }
            curr = curr->dictionary[dict[i][j] - 'a'];
        }
        curr->ind = i;
        curr->isWord = true;
    }
}

bool spell_check(Node* root, string& word) {
    Node* curr = root;
    for (int i = 0; i < word.size(); i++) {
        if (curr->dictionary[word[i] - 'a'] == NULL)
            return false;
        else {
            curr = curr->dictionary[word[i] - 'a'];
        }
    }
    if (curr->isWord) return true;
    return false;
}

int main() {
    int n, q;
    cin >> n >> q;

    vector<string> dict(n);
    for (int i = 0; i < n; i++) {
        cin >> dict[i];
    }
    Node* root = new Node();
    buildTrie(root, dict);

    string str = "dame";
    cout << spell_check(root, str) << endl;

    return 0;
}

/*
10 4
consider
filters
filers
entitled
tilers
litter
dames
filling
grasses
fitter

 */