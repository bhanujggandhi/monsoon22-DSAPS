#include <iostream>
#include <vector>

using namespace std;

#define ALPHABET 256;

std::string T;

struct Pair {
    int first;
    int second;

    Pair(int _f, int _s) {
        first = _f;
        second = _s;
    }
};

std::vector<int> counting_sort_build_suffix() {
    int n = T.size();
    int alpha = ALPHABET;
    std::vector<int> p(n), r(n), count_arr(std::max(alpha, n), 0);

    for (size_t i = 0; i < n; i++) {
        count_arr[T[i]]++;
    }

    for (size_t i = 1; i < count_arr.size(); i++) {
        count_arr[i] += count_arr[i - 1];
    }

    for (size_t i = 0; i < n; i++) {
        count_arr[T[i]]--;
        p[count_arr[T[i]]] = i;
    }

    // Rank
    r[p[0]] = 0;
    int rank = 1;
    for (size_t i = 1; i < n; i++) {
        if (T[p[i]] != T[p[i - 1]])
            rank++;
        r[p[i]] = rank - 1;
    }

    std::vector<int> tempp(n), tempc(n);

    for (int h = 0; (1 << h) < n; ++h) {
        for (int i = 0; i < n; i++) {
            tempp[i] = p[i] - (1 << h);
            if (tempp[i] < 0)
                tempp[i] += n;
        }
        fill(count_arr.begin(), count_arr.begin() + rank, 0);
        for (int i = 0; i < n; i++)
            count_arr[r[tempp[i]]]++;
        for (int i = 1; i < rank; i++)
            count_arr[i] += count_arr[i - 1];
        for (int i = n - 1; i >= 0; i--)
            p[--count_arr[r[tempp[i]]]] = tempp[i];
        tempc[p[0]] = 0;
        rank = 1;
        for (int i = 1; i < n; i++) {
            Pair cur(r[p[i]], r[(p[i] + (1 << h)) % n]);
            Pair prev(r[p[i - 1]], r[(p[i - 1] + (1 << h)) % n]);
            if (cur.first != prev.first or cur.second != prev.second)
                ++rank;
            tempc[p[i]] = rank - 1;
        }
        r.swap(tempc);
    }
    return p;
}

int main() {
    std::string inp;
    cin >> inp;
    int sz = inp.size();
    T = inp + inp;

    std::vector<int> suffix_array = counting_sort_build_suffix();
    int n = T.size();

    for (auto x : suffix_array) {
        if ((n - x + 1) > sz) {
            cout << T.substr(x).substr(0, sz) << "\n";
            break;
        }
    }
}