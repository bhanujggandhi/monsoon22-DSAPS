#include <bits/stdc++.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

typedef long long ll;

ll find_set(ll v, vector<ll> &parent) {
    if (parent[v] == v)
        return v;

    return parent[v] = find_set(parent[v], parent);
}

void union_set(ll a, ll b, vector<ll> &parent, vector<ll> &maximus) {
    a = find_set(a, parent);
    b = find_set(b, parent);

    if (a != b) {
        if (maximus[a] < maximus[b]) {
            parent[a] = b;
            maximus[b] += maximus[a];

        } else {
            parent[b] = a;
            maximus[a] += maximus[b];
        }
    }
}

int main() {

    ll n, p;
    cin >> n >> p;
    vector<ll> parent(n);
    vector<ll> maximus(n);
    for (int i = 0; i < n; i++) {
        parent[i] = i;
        maximus[i] = 1;
    }

    ll totalpairs = (n * (n - 1)) / 2;

    while (p--) {
        ll a, b;
        cin >> a >> b;
        union_set(a, b, parent, maximus);
    }

    //     ll ans = 0;

    //     for(ll i = 0; i < n-1; i++)
    //     {
    //         for(ll j = i+1; j < n; j++)
    //         {
    //             if(find_set(i, parent) != find_set(j, parent))
    //             {
    //                 ans++;
    //             }
    //         }
    //     }

    for (int i = 0; i < n; i++) {
        if (i == parent[i]) {
            totalpairs -= ((maximus[parent[i]] * (maximus[parent[i]] - 1)) / 2);
        }
    }

    cout << totalpairs << endl;

    return 0;
}
