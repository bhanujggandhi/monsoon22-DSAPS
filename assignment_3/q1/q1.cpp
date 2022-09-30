#include <iostream>
#include <string>
#include <vector>

using namespace std;

/// @brief Node structure for Trie
struct Node {
    Node* dictionary[26];
    bool isWord;
    int ind;
};

/// @brief Function to add word to the trie
/// @param root
/// @param word
void addWord(Node* root, string& word) {
    Node* curr = root;
    for (int j = 0; j < word.length(); j++) {
        if (curr->dictionary[word[j] - 'a'] == NULL) {
            curr->dictionary[word[j] - 'a'] = new Node();
        }
        curr = curr->dictionary[word[j] - 'a'];
    }
    curr->isWord = true;
}

/// @brief Function that checks if the word is spelled correct by checking if
/// the entered word is present in the dictionary
/// @param root
/// @param word
/// @return Boolean True if present, otherwise false
bool spellCheck(Node* root, string& word) {
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

/// @brief Utility function to get all the words present after a certain Trie
/// Node
/// @param root
/// @param suggestions
/// @param currword
void getWords(Node* root, vector<string>& suggestions, string currword) {
    if (root->isWord) suggestions.push_back(currword);

    for (int i = 0; i < 26; i++) {
        if (root->dictionary[i] == NULL) continue;

        currword.push_back(i + 'a');
        getWords(root->dictionary[i], suggestions, currword);
        currword.pop_back();
    }
}

/// @brief Function to give autocomplete suggestions when a part of the word is
/// entered
/// @param root
/// @param word
/// @return Array of the suggestions of autocomplete of the word
vector<string> autocomplete(Node* root, string& word) {
    vector<string> suggestions;
    Node* currentNode = root;
    string currword;
    for (int i = 0; i < word.size(); i++) {
        if (currentNode->dictionary[word[i] - 'a'] == NULL)
            return suggestions;
        else {
            currword.push_back(word[i]);
            currentNode = currentNode->dictionary[word[i] - 'a'];
        }
    }

    Node* temp = currentNode;
    for (int i = 0; i < 26; i++) {
        if (temp->dictionary[i] == NULL) continue;

        currword.push_back(i + 'a');
        getWords(temp->dictionary[i], suggestions, currword);
        currword.pop_back();
    }

    return suggestions;
}

/// @brief Utility find auto corrected word by calculating Levenshein Distance
/// upto 3
/// @param root
/// @param c
/// @param word
/// @param prevRow
/// @param ans
/// @param currword
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

/// @brief Function that gets all the suggestions for autocorrect the entered
/// word
/// @param root
/// @param word
/// @return Array of suggestions for Autocorrect Words
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

/// @brief Driver Code
/// @return Returns Status of the Program
int main() {
    int n, q;
    cin >> n >> q;

    Node* root = new Node();
    for (int i = 0; i < n; i++) {
        string word;
        cin >> word;
        addWord(root, word);
    }

    while (q--) {
        int a;
        cin >> a;
        string word;
        cin >> word;

        if (a == 1) {
            cout << spellCheck(root, word) << endl;
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
