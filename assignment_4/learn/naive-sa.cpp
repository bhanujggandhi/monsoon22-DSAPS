#include <bits/stdc++.h>

using namespace std;

string T;

bool cmp(int a, int b) {
    return T.substr(a) < T.substr(b);
}

int main() {

    T = "GATAGACA$";
    int n = T.size();

    vector<int> SA(n);
    for (int i = 0; i < n; i++) {
        SA[i] = i;
    }

    sort(SA.begin(), SA.end(), cmp);

    for (auto x : SA) {
        cout << x << " ";
    }

    cout << "\n";

    return 0;
}