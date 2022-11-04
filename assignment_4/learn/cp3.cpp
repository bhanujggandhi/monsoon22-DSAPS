#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;

std::string T;

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

void build_suffix_array(std::vector<Item> &suffixarr) {
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
        std::vector<int> trav(n);
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
            if ((suffixarr[j].index + i) < n) {
                suffixarr[j].ra2 = suffixarr[trav[suffixarr[j].index + i]].ra1;
            } else
                suffixarr[j].ra2 = -1;
        }

        if (suffixarr[n - 1].ra1 == n - 1) break;
    }
}

int main() {
    cin >> T;
    int n = T.size();
    std::vector<Item> suffix_array;

    build_suffix_array(suffix_array);

    for (auto x : suffix_array) {
        cout << x.index << " ";
    }
}