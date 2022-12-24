#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

#define ALPHABET 256;

class SegmentTree {
   private:
    vector<int> segment_arr;

   public:
    SegmentTree(int n) {
        segment_arr.resize(4 * n);
    }

    void build(int ind, int low, int high, vector<int> &arr) {
        if (low == high) {
            segment_arr[ind] = arr[low];
            return;
        }

        int mid = low + (high - low) / 2;
        build((2 * ind) + 1, low, mid, arr);
        build((2 * ind) + 2, mid + 1, high, arr);
        segment_arr[ind] = min(segment_arr[(2 * ind) + 1], segment_arr[(2 * ind) + 2]);
    }

    int query(int ind, int low, int high, int lind, int rind) {

        if (rind < low or high < lind) {
            return INT32_MAX;
        }

        if (low >= lind and high <= rind) {
            return segment_arr[ind];
        }

        int mid = low + (high - low) / 2;
        int l = query(2 * ind + 1, low, mid, lind, rind);
        int r = query(2 * ind + 2, mid + 1, high, lind, rind);

        return min(l, r);
    }

    void point_update(int ind, int low, int high, int i, int val) {
        if (low == high) {
            segment_arr[ind] = val;
            return;
        }

        int mid = low + (high - low) / 2;
        if (i <= mid)
            point_update(2 * ind + 1, low, mid, i, val);
        else
            point_update(2 * ind + 2, mid + 1, high, i, val);

        segment_arr[ind] = min(segment_arr[2 * ind + 1], segment_arr[2 * ind + 2]);
    }
};

std::string T;

string reverseStr(string &str) {
    int n = str.length();
    string ans = str;

    for (size_t i = 0; i < n / 2; i++)
        swap(ans[i], ans[n - i - 1]);

    return ans;
}

void contruct_lcp_array(vector<int> &sa, vector<int> &rankarr, vector<int> &lcparr) {
    int n = T.size();
    int j, k = 0;
    for (size_t i = 1; i <= n; i++) {
        int a = sa[i];
        rankarr[a] = i;
    }
    for (size_t i = 0; i < n; i++) {
        if (k) {
            k--;
        }
        int tempe = rankarr[i] - 1;
        j = sa[tempe];
        while (T[i + k] == T[j + k])
            k++;
        int temp_a = i;
        int temp_b = rankarr[temp_a];
        lcparr[temp_b] = k;
    }
}

struct Item {
    int index;
    int ra1;
    int ra2;
};

int cmprtr(struct Item a, struct Item b) {
    if (a.ra1 == b.ra1)
        return a.ra2 < b.ra2;

    return a.ra1 < b.ra1;
}

void buildSuffixArray(std::vector<int> &sa) {
    int n = T.size();
    struct Item suffixa[n];

    for (size_t i = 0; i < n; i++) {
        suffixa[i].index = i;
        suffixa[i].ra1 = T[i] - 'a';
        suffixa[i].ra2 = ((i + 1) < n) ? (T[i + 1] - 'a') : -1;
    }
    std::sort(suffixa, suffixa + n, cmprtr);
    int ind[n];

    for (size_t k = 4; k < 2 * n; k *= 2) {
        int rank = 0;
        int prevrank = suffixa[0].ra1;
        suffixa[0].ra1 = rank;
        ind[suffixa[0].index] = 0;

        for (size_t i = 1; i < n; i++) {
            if (suffixa[i].ra1 == prevrank && suffixa[i].ra2 == suffixa[i - 1].ra2) {
                prevrank = suffixa[i].ra1;
                suffixa[i].ra1 = rank;
            } else {
                prevrank = suffixa[i].ra1;
                suffixa[i].ra1 = (++rank);
            }

            ind[suffixa[i].index] = i;
        }

        for (size_t i = 0; i < n; i++) {
            int nextindex = suffixa[i].index + k / 2;
            suffixa[i].ra2 = (nextindex < n) ? suffixa[ind[nextindex]].ra1 : -1;
        }

        std::sort(suffixa, suffixa + n, cmprtr);
    }

    for (size_t i = 0; i < n; i++)
        sa[i] = suffixa[i].index;
}

int main() {
    cin >> T;
    int m = T.size();

    bool flag = true;
    for (size_t i = 0; i < T.length() / 2; i++) {

        if (T[i] != T[T.length() - i - 1]) {
            flag = false;
            break;
        }
    }

    if (flag) {
        cout << T << "\n";
        return 0;
    }

    string rev = reverseStr(T);

    T = T + "$" + rev + "#";
    int n = T.size();

    std::vector<int> sa(n);
    buildSuffixArray(sa);

    vector<int> rankarr(n, 0);

    for (size_t i = 0; i < n; i++) {
        rankarr[sa[i]] = i;
    }

    vector<int> lcparr(n);
    contruct_lcp_array(sa, rankarr, lcparr);

    SegmentTree sg(lcparr.size());

    sg.build(0, 0, lcparr.size() - 1, lcparr);

    int max_ind = -1;
    int max_len = 0;

    for (size_t i = 0; i < m; i++) {
        int prevind = rankarr[n - i - 2];
        int k = rankarr[i];

        int len = 0;
        if (prevind > k) {
            len = sg.query(0, 0, lcparr.size() - 1, k + 1, prevind);
        } else if (prevind == k) {
            len = sg.query(0, 0, lcparr.size() - 1, k, prevind);

        } else {
            len = sg.query(0, 0, lcparr.size() - 1, prevind + 1, k);
        }

        if (len != 0 and (2 * len - 1) >= max_len) {
            if (max_ind == -1 or ((2 * len - 1) > max_len) or (max_len == (2 * len - 1) and rankarr[i - len + 1] < rankarr[max_ind])) {
                max_len = (2 * len) - 1;
                max_ind = i - len + 1;
            }
        }
    }

    for (size_t i = 0; i < m - 1; i++) {
        int prevind = rankarr[n - i - 2];
        int k = rankarr[i + 1];

        int len = 0;
        if (prevind > k) {
            len = sg.query(0, 0, lcparr.size() - 1, k + 1, prevind);
        } else {
            len = sg.query(0, 0, lcparr.size() - 1, prevind + 1, k);
        }

        if (len != 0 and (2 * len) >= max_len) {
            if (max_ind == -1 or ((2 * len) > max_len) or (max_len == (2 * len) and rankarr[i - len + 1] < rankarr[max_ind])) {
                max_len = (2 * len);
                max_ind = i - len + 1;
            }
        }
    }

    cout << T.substr(max_ind, max_len) << endl;

    return 0;
}