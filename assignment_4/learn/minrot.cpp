
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

#define ALPHABET 256;

using namespace std;

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

int main() {
    std::string inp;
    cin >> inp;
    int sz = inp.size();
    T = inp + inp;
    // T.push_back('$');

    std::vector<int> suffix_array;
    suffix_array = counting_sort_build_suffix(suffix_array);
    int n = T.size();

    for (auto x : suffix_array) {
        if ((n - x + 1) > sz) {
            cout << T.substr(x).substr(0, sz) << "\n";
            break;
        }
    }
}