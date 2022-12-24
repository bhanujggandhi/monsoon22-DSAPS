#include <bits/stdc++.h>
#include <deque>
#include <iostream>
#include <utility>
#include <vector>

using std::cin;
using std::cout;
using std::endl;

#define ALPHABET 256;

std::string T;

std::vector<int> counting_sort_build_suffix(std::vector<int> ssort) {
    int n = T.size();
    int alpha = ALPHABET;
    std::vector<int> p(n), c(n), cnt(std::max(alpha, n), 0);

    for (size_t i = 0; i < n; i++) {
        cnt[T[i]]++;
    }

    for (size_t i = 1; i < cnt.size(); i++) {
        cnt[i] += cnt[i - 1];
    }

    for (size_t i = 0; i < n; i++) {
        cnt[T[i]]--;
        p[cnt[T[i]]] = i;
    }

    // Rank
    c[p[0]] = 0;
    int classes = 1;
    for (size_t i = 1; i < n; i++) {
        if (T[p[i]] != T[p[i - 1]])
            classes++;
        c[p[i]] = classes - 1;
    }

    std::vector<int> pn(n), cn(n);

    for (int h = 0; (1 << h) < n; ++h) {
        for (int i = 0; i < n; i++) {
            pn[i] = p[i] - (1 << h);
            if (pn[i] < 0)
                pn[i] += n;
        }
        fill(cnt.begin(), cnt.begin() + classes, 0);
        for (int i = 0; i < n; i++)
            cnt[c[pn[i]]]++;
        for (int i = 1; i < classes; i++)
            cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; i--)
            p[--cnt[c[pn[i]]]] = pn[i];
        cn[p[0]] = 0;
        classes = 1;
        for (int i = 1; i < n; i++) {
            std::pair<int, int> cur = {c[p[i]], c[(p[i] + (1 << h)) % n]};
            std::pair<int, int> prev = {c[p[i - 1]], c[(p[i - 1] + (1 << h)) % n]};
            if (cur != prev)
                ++classes;
            cn[p[i]] = classes - 1;
        }
        c.swap(cn);
    }
    return p;
}

std::vector<int> lcp_construction(std::string const &s, std::vector<int> const &p) {
    int n = s.size();
    std::vector<int> rank(n, 0);
    for (int i = 0; i < n; i++)
        rank[p[i]] = i;

    int k = 0;
    std::vector<int> lcp(n - 1, 0);
    for (int i = 1; i < n; i++) {
        if (rank[i] == n - 1) {
            k = 0;
            continue;
        }
        int j = p[rank[i] + 1];
        while (i + k < n && j + k < n && s[i + k] == s[j + k])
            k++;
        lcp[rank[i]] = k;
        if (k)
            k--;
    }
    return lcp;
}

std::vector<int> kasai(std::vector<int> &suffixArr) {
    int n = suffixArr.size();

    std::vector<int> lcp(n, 0);

    std::vector<int> invSuff(n, 0);
    for (int i = 0; i < n; i++)
        invSuff[suffixArr[i]] = i;

    int k = 0;

    for (int i = 0; i < n; i++) {
        if (invSuff[i] == n - 1) {
            k = 0;
            continue;
        }

        int j = suffixArr[invSuff[i] + 1];

        while (i + k < n && j + k < n && T[i + k] == T[j + k])
            k++;

        lcp[invSuff[i]] = k;

        if (k > 0)
            k--;
    }

    return lcp;
}

std::vector<int> kasai2(std::string &s, std::vector<int> &sa) {
    int n = s.size(), k = 0;
    std::vector<int> lcp(n, 0);
    std::vector<int> rank(n, 0);

    for (int i = 0; i < n; i++) rank[sa[i]] = i;

    for (int i = 0; i < n; i++, k ? k-- : 0) {
        if (rank[i] == n - 1) {
            k = 0;
            continue;
        }
        int j = sa[rank[i] + 1];
        while (i + k < n && j + k < n && s[i + k] == s[j + k]) k++;
        lcp[rank[i]] = k;
    }
    return lcp;
}

// int find_longest_string_K_times(int k, std::vector<int> &lcp) {
//     int res = -1e8;
//     Deque<int> dq;
//     dq.deque();
//     for (int i = 0; i < k; ++i) {
//         if (!dq.empty()) {
//             while (!dq.empty() && lcp[i] < lcp[dq.back()]) {
//                 dq.pop_back();
//             }
//         }

//         dq.push_back(i);
//     }

//     res = std::max(res, lcp[dq.front()]);
//     for (int i = k; i < lcp.size(); i++) {
//         while (!dq.empty() && dq.front() < (i - k + 1)) {
//             dq.pop_front();
//         }
//         if (!dq.empty()) {
//             while (!dq.empty() && lcp[i] < lcp[dq.back()]) {
//                 dq.pop_back();
//             }
//         }
//         dq.push_back(i);

//         res = std::max(res, lcp[dq.front()]);
//         // cout << dq.front() << "   ";
//     }

//     return res;
// }

std::vector<int> maxSlidingWindow(std::vector<int> &nums, int k) {
    int n = nums.size();
    std::vector<int> maxLeft(n);
    std::vector<int> maxRight(n);
    std::vector<int> ans(n - k + 1);
    maxLeft[0] = nums[0];
    maxRight[n - 1] = nums[n - 1];
    for (int i = 1; i < n; i++) {
        maxLeft[i] = (i % k == 0) ? nums[i] : std::min(maxLeft[i - 1], nums[i]);
        int j = n - i - 1;
        maxRight[j] = (j % k == 0) ? nums[j] : std::min(maxRight[j + 1], nums[j]);
    }

    for (int i = 0, j = 0; i + k <= n; i++) {
        ans[j++] = std::min(maxRight[i], maxLeft[i + k - 1]);
    }

    return ans;
}

void buildLCP(std::vector<int> &SA_, std::vector<int> &rank, std::vector<int> &lcp, std::string s, int n) {
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

    // int *suffixArr = new int[n];
    for (int i = 0; i < n; i++)
        sa[i] = suffixes[i].index;

    // Return the suffix array
    // return suffixArr;
}

int main() {
    int n, k;
    cin >> k >> n;
    cin >> T;
    n = T.size();

    std::vector<int> sa(n);
    buildSuffixArray(T, n, sa);
    // sa = counting_sort(sa);

    std::vector<int> rankarr(n, 0);

    for (size_t i = 0; i < n; i++) {
        rankarr[sa[i]] = i;
    }

    std::vector<int> lcparr = kasai2(T, sa);
    // buildLCP(sa, rankarr, lcparr, T, T.size());
    // for (auto x : sa) {
    //     cout << x << "  ";
    // }
    // cout << "\n";

    // for (auto x : rankarr) {
    //     cout << x << "  ";
    // }
    // cout << "\n";
    // for (auto x : lcparr) {
    //     cout << x << "  ";
    // }
    // cout << "\n";
    // int ans = find_longest_string_K_times(k - 1, lcparr);
    std::vector<int> mins = maxSlidingWindow(lcparr, k - 1);

    int ans = 0;
    for (auto x : mins) {
        ans = std::max(ans, x);
    }

    // cout << larr[68800] << endl;

    if (ans == 0) {
        ans = -1;
    }

    cout << ans << "\n";

    return 0;
}