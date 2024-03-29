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

    for (int i = 0; i < n / 2; i++)
        swap(ans[i], ans[n - i - 1]);

    return ans;
}

void contruct_lcp_array(vector<int> &SA_, vector<int> &rank, vector<int> &lcp, string &s, int n) {
    int i, j, k = 0;
    for (i = 1; i <= n; i++) {
        int a = SA_[i];
        rank[a] = i;
    }
    for (i = 0; i < n;) {
        if (k) {
            k--;
        }
        int temp_d = rank[i];
        int temp_e = temp_d - 1;
        j = SA_[temp_e];
        for (; s[i + k] == s[j + k]; k++) {
            ;
        }
        int temp_a = i;
        int temp_b = rank[temp_a];
        lcp[temp_b] = k;
        i++;
    }
    return;
}

struct Item {
    int index;
    int s[2];
};

int cmp(struct Item a, struct Item b) {
    int temp = 0;
    if (a.s[0] == b.s[0]) {
        temp = (a.s[1] < b.s[1] ? 1 : 0);
    } else
        temp = (a.s[0] < b.s[0] ? 1 : 0);
    return temp;
}

void buildSuffixArray(std::string &text, int n, std::vector<int> &sa) {
    struct Item suffixes[n];

    for (int i = 0; i < n; ++i) {
        suffixes[i].index = i;
        suffixes[i].s[0] = text[i] - 'a';
        suffixes[i].s[1] = ((i + 1) < n) ? (text[i + 1] - 'a') : -1;
    }
    std::sort(suffixes, suffixes + n, cmp);
    int ind[n];
    for (int k = 4; k < 2 * n; k *= 2) {
        int rank = 0;
        int prev_rank = suffixes[0].s[0];
        suffixes[0].s[0] = rank;
        ind[suffixes[0].index] = 0;

        for (int i = 1; i < n; ++i) {
            if (suffixes[i].s[0] == prev_rank && suffixes[i].s[1] == suffixes[i - 1].s[1]) {
                prev_rank = suffixes[i].s[0];
                suffixes[i].s[0] = rank;
            }

            else {
                prev_rank = suffixes[i].s[0];
                ++rank;
                suffixes[i].s[0] = rank;
            }

            ind[suffixes[i].index] = i;
        }

        for (int i = 0; i < n; ++i) {
            int nextindex = suffixes[i].index + k / 2;
            suffixes[i].s[1] = (nextindex < n) ? suffixes[ind[nextindex]].s[0] : -1;
        }

        std::sort(suffixes, suffixes + n, cmp);
    }

    for (int i = 0; i < n; i++)
        sa[i] = suffixes[i].index;
}

// aabaca
// abbaca
int main() {
    int z;
    cin >> z;
    cin >> T;
    int m = T.size();

    bool flag = true;
    for (int i = 0; i < T.length() / 2; i++) {

        if (T[i] != T[T.length() - i - 1]) {
            flag = false;
            break;
        }
    }

    if (flag) {
        cout << T.size() << "\n";
        return 0;
    }

    string rev = reverseStr(T);

    T = T + "$" + rev + "#";
    int n = T.size();

    std::vector<int> sa(n);
    buildSuffixArray(T, n, sa);

    vector<int> rankarr(n, 0);

    for (size_t i = 0; i < n; i++) {
        rankarr[sa[i]] = i;
    }

    vector<int> lcparr(n);
    contruct_lcp_array(sa, rankarr, lcparr, T, T.size());

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

    cout << max_len << endl;

    return 0;
}