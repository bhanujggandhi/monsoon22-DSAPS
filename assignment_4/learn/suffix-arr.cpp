#include <bits/stdc++.h>

using namespace std;

string T;

struct Item {
    int index;
    int ra1;
    int ra2;
    Item(int _i, int _ra1, int _ra2) {
        index = _i;
        ra1 = _ra1;
        ra2 = _ra2;
    }
};

bool cmp(Item a, Item b) {
    if (a.ra1 == b.ra1)
        return a.ra2 < b.ra2;

    return a.ra1 < b.ra1;
}

void build_suffix_array(vector<Item> &suffixarr) {
    int n = T.size();
    for (int i = 0; i < T.size(); i++) {
        Item temp(i, T[i], i + 1 < n ? T[i + 1] : -1);
        suffixarr.push_back(temp);
    }

    for (int i = 1; i < n; i *= 2) {
        sort(suffixarr.begin(), suffixarr.end(), cmp);
        int r = 0;
        int prev = suffixarr[0].ra1;
        suffixarr[0].ra1 = 0;
        vector<int> trav(n);
        trav[suffixarr[0].index] = 0;
        for (int j = 1; j < n; j++) {
            if (suffixarr[j].ra1 == prev and suffixarr[j].ra2 == suffixarr[j - 1].ra2) {
                suffixarr[j].ra1 = r;
            } else {
                prev = suffixarr[j].ra1;
                suffixarr[j].ra1 = ++r;
            }
            trav[suffixarr[j].index] = j;
        }

        for (int j = 0; j < n; j++) {
            suffixarr[j].ra2 = ((suffixarr[j].index + i) < n) ? suffixarr[trav[suffixarr[j].index + i]].ra1 : -1;
        }

        if (suffixarr[n - 1].ra1 == n - 1) break;
    }
}

void find_lcp(vector<Item> &suffix_array, vector<int> &lcp) {
    int n = T.size();
    vector<int> prev(n);
    prev[0] = -1;

    for (int i = 1; i < n; i++) {
        prev[suffix_array[i].index] = suffix_array[i - 1].index;
    }

    vector<int> plcp(n);

    int ind = 0;
    for (int i = 0; i < n; i++) {
        if (prev[i] == -1) {
            plcp[i] = 0;
            continue;
        }

        while (T[i + ind] == T[prev[i] + ind]) ind++;
        plcp[i] = ind;
        ind = max(ind - 1, 0);
    }

    for (int i = 0; i < n; i++) {
        lcp[i] = plcp[suffix_array[i].index];
    }
}

int main() {
    T = "abrakadabra";
    int n = T.size();

    vector<Item> suffix_array;

    build_suffix_array(suffix_array);

    // vector<int> lcp(n);
    // find_lcp(suffix_array, lcp);
    // for (int i = 0; i < n; i++) {
    //     cout << suffix_array[i].index << "-->" << T.substr(suffix_array[i].index) << "-->" << lcp[i]
    //          << "\n";
    // }
    int k = 2;

    int ans = 0;
    for (int i = 0; i < 1 + n - k; i++) {
        int len = 0;
        int j = suffix_array[i].index;
        int l = suffix_array[i + k - 1].index;
        while (T[j] == T[l] && j < n && l < n) {
            len++;
            l = l + 1;
            j = j + 1;
        }
        if (ans < len)
            ans = len;
    }

    cout << ans << endl;

    return 0;
}
