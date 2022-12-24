#include <bits/stdc++.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

long long findPar(long long u, vector<long long> &parent) {
    if (parent[u] == u) {
        return u;
    }
    return parent[u] = findPar(parent[u], parent);
}

void unionn(long long u, long long v, vector<long long> &parent, vector<long long> &rank) {
    u = findPar(u, parent);
    v = findPar(v, parent);
    if (u != v) {
        if (rank[u] < rank[v])
            swap(u, v);
        parent[v] = u;
        if (rank[u] == rank[v])
            rank[u]++;
    }
}

int main() {

    long long n, p;
    cin >> n >> p;
    vector<long long> parent(n);
    vector<long long> rank(n, 0);

    for (long long i = 0; i < n; i++) {
        parent[i] = i;
    }

    for (long long i = 0; i < p; i++) {
        long long u, v;
        cin >> u >> v;
        if (findPar(u, parent) != findPar(v, parent)) {
            unionn(u, v, parent, rank);
        }
    }

    unordered_map<long long, long long> mpp;

    for (long long i = 0; i < n; i++) {
        findPar(i, parent);
        mpp[parent[i]]++;
    }

    long long total = (n * (n - 1)) / 2;
    for (auto it : mpp) {
        total -= (it.second * (it.second - 1)) / 2;
    }
    for (auto x : mpp) {
        cout << x.first << "-->" << x.second << endl;
    }
    cout << total << endl;

    return 0;
}