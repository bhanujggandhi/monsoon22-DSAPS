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

void getWords(Node* root, vector<string>& suggestions, string currword) {
    if (root->isWord) suggestions.push_back(currword);

    for (int i = 0; i < 26; i++) {
        if (root->dictionary[i] == NULL) continue;

        currword.push_back(i + 'a');
        getWords(root->dictionary[i], suggestions, currword);
        currword.pop_back();
    }
}

vector<string> autocomplete(Node* root, string& word) {
    vector<string> suggestions;
    Node* curr = root;
    string currword;
    for (int i = 0; i < word.size(); i++) {
        if (curr->dictionary[word[i] - 'a'] == NULL)
            return suggestions;
        else {
            currword.push_back(word[i]);
            curr = curr->dictionary[word[i] - 'a'];
        }
    }

    Node* temp = curr;
    for (int i = 0; i < 26; i++) {
        if (temp->dictionary[i] == NULL) continue;

        currword.push_back(i + 'a');
        getWords(temp->dictionary[i], suggestions, currword);
        currword.pop_back();
    }

    return suggestions;
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

    string str = "fi";

    cout << "---------------" << endl;
    vector<string> ans = autocomplete(root, str);
    cout << ans.size() << endl;
    for (auto x : ans) cout << x << " ";

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