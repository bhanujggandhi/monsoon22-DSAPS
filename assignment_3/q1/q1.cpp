#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Node {
    Node* dictionary[26];
    bool isWord;
    int ind;
};

void buildTrie(Node* root, string& word) {
    Node* curr = root;
    for (int j = 0; j < word.length(); j++) {
        if (curr->dictionary[word[j] - 'a'] == NULL) {
            curr->dictionary[word[j] - 'a'] = new Node();
        }
        curr = curr->dictionary[word[j] - 'a'];
    }
    curr->isWord = true;
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

void autocorrectHelper(Node* root, char c, string& word, vector<int>& prevRow,
                       vector<string>& ans, string& currword) {
    vector<int> t;
    t.push_back(prevRow[0] + 1);

    for (int i = 1; i < word.size() + 1; i++) {
        int insertop = t[i - 1] + 1;
        int deleteop = prevRow[i] + 1;
        int replaceop = 0;
        if (word[i - 1] != c) {
            replaceop = prevRow[i - 1] + 1;
        } else
            replaceop = prevRow[i - 1];

        t.push_back(min(insertop, min(deleteop, replaceop)));
    }

    if (t[word.size()] <= 3 and root->isWord) {
        ans.push_back(currword);
    }

    int currminentry = INT32_MAX;
    for (auto x : t) currminentry = min(x, currminentry);

    if (currminentry <= 3) {
        for (int i = 0; i < 26; i++) {
            if (root->dictionary[i] != NULL) {
                char temp = i + 'a';
                currword.push_back(temp);
                autocorrectHelper(root->dictionary[i], temp, word, t, ans,
                                  currword);
                currword.pop_back();
            }
        }
    }
}

vector<string> autocorrect(Node* root, string& word) {
    vector<int> t(word.size() + 1);

    // Top row
    for (int i = 0; i < t.size(); i++) {
        t[i] = i;
    }

    vector<string> ans;

    string currword;
    for (int i = 0; i < 26; i++) {
        if (root->dictionary[i] != NULL) {
            char temp = i + 'a';
            currword.push_back(temp);
            autocorrectHelper(root->dictionary[i], temp, word, t, ans,
                              currword);
            currword.pop_back();
        }
    }

    return ans;
}

int main() {
    int n, q;
    cin >> n >> q;

    Node* root = new Node();
    for (int i = 0; i < n; i++) {
        string word;
        cin >> word;
        buildTrie(root, word);
    }

    while (q--) {
        int a;
        cin >> a;
        string word;
        cin >> word;

        if (a == 1) {
            cout << spell_check(root, word) << endl;
        } else if (a == 2) {
            vector<string> suggestions;
            suggestions = autocomplete(root, word);
            cout << suggestions.size() << endl;
            for (int i = 0; i < suggestions.size(); i++) {
                cout << suggestions[i] << endl;
            }
        } else if (a == 3) {
            vector<string> suggestions;
            suggestions = autocorrect(root, word);
            cout << suggestions.size() << endl;
            for (int i = 0; i < suggestions.size(); i++) {
                cout << suggestions[i] << endl;
            }
        }
    }

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
1 litter
1 dame
2 con
3 filter
 */
