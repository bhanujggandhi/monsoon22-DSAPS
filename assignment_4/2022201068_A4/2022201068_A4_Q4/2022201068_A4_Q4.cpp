#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int find_set(int v, int parent[]) {
    if (v == parent[v]) {
        return v;
    }
    return parent[v] = find_set(parent[v], parent);
}

void union_set(int a, int b, int parent[], int maximus[], int* size,
               int* groups) {
    a = find_set(a, parent);
    b = find_set(b, parent);

    if (a != b) {
        if (maximus[a] < maximus[b]) {
            parent[a] = b;
            maximus[b] += maximus[a];
            *size = std::max(*size, maximus[b]);
        } else {
            parent[b] = a;
            maximus[a] += maximus[b];
            *size = std::max(*size, maximus[a]);
        }
        *groups = *groups - 1;
    }
}

int main() {

    int parties, alliances;
    cin >> parties >> alliances;
    int maxsize = 1;
    int groups = parties;
    int sizeset[parties];

    int parent[parties + 1];

    parent[0] = -1;
    sizeset[0] = -1;

    for (int i = 1; i < parties + 1; i++) {
        parent[i] = i;
        sizeset[i] = 1;
    }

    while (alliances--) {
        int a, b;
        cin >> a >> b;

        union_set(a, b, parent, sizeset, &maxsize, &groups);

        cout << groups << " " << maxsize << endl;
    }

    return 0;
}