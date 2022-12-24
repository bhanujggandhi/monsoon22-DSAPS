#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;

struct Node {
    int node;
    int weight;
    Node *prev;
    Node *next;

    Node(int _node, int _weight) {
        node = _node;
        weight = _weight;
        prev = NULL;
        next = NULL;
    }
};

struct Pair {
    int node;
    int weight;
};

/* 1 based indexing Heap */
class MinHeap {
   private:
    std::vector<Pair> arr;

    void heapify(int idx) {
        int left = 2 * idx;
        int right = 2 * idx + 1;

        int minIdx = idx;

        if (left < arr.size() and arr[left].weight < arr[idx].weight) {
            minIdx = left;
        }

        if (right < arr.size() and arr[right].weight < arr[minIdx].weight) {
            minIdx = right;
        }

        if (minIdx != idx) {
            std::swap(arr[idx], arr[minIdx]);
            heapify(minIdx);
        }
    }

   public:
    MinHeap(int size = 8) {
        arr.reserve(size + 1);
        arr.push_back({-1, -1});
    }

    void push(int node, int weight) {
        arr.push_back({node, weight});

        int idx = arr.size() - 1;
        int parent = idx / 2;

        while (idx > 1 and arr[idx].weight < arr[parent].weight) {
            std::swap(arr[idx], arr[parent]);
            idx = parent;
            parent = parent / 2;
        }
    }

    Pair top() {
        if (arr.size() == 1) return {-1, -1};
        return arr[1];
    }

    void pop() {
        if (arr.size() == 1) return;
        int idx = arr.size() - 1;
        std::swap(arr[1], arr[idx]);
        arr.pop_back();
        heapify(1);
    }

    bool isEmpty() { return arr.size() == 1; }
};

struct List {
    Node *front;
    Node *rear;
    int size;

    List() {
        size = 0;
        front = new Node(-1, -1);
        rear = new Node(-1, -1);
        front->next = rear;
        rear->prev = front;
    }

    ~List() {
        Node *temp;
        while (front != NULL) {
            temp = front;
            delete temp;
            front = front->next;
        }
    }

    void add(Node *data) {
        front->next->prev = data;
        data->next = front->next;
        front->next = data;
        data->prev = front;
        this->size++;
    }

    void remove(Node *data) {
        data->prev->next = data->next;
        data->next->prev = data->prev;
        this->size--;
    }
};

void dijkstra(std::vector<List *> &ajdlist, int src, std::vector<int> &ans,
              MinHeap &miniH) {

    auto t = ajdlist[src]->front->next;
    while (t != ajdlist[src]->rear) {
        int adj = t->node;
        if (ans[src] + t->weight < ans[adj]) {
            ans[adj] = ans[src] + t->weight;
            miniH.push(adj, ans[adj]);
        }
        t = t->next;
    }

    if (miniH.isEmpty()) return;

    Pair newsrc = miniH.top();
    miniH.pop();

    dijkstra(ajdlist, newsrc.node, ans, miniH);
}

void nearest_station(std::vector<List *> &ajdlist,
                     std::vector<int> &policestations) {
    MinHeap miniH;
    std::vector<int> ans(ajdlist.size(), 1e8 + 7);
    for (uint i = 0; i < policestations.size(); i++) {
        miniH.push(policestations[i], 0);
        ans[policestations[i]] = 0;
    }

    for (uint i = 1; i < ans.size(); i++) {
        if (ans[i] == 1e8 + 7) miniH.push(i, 1e8 + 7);
    }

    Pair src = miniH.top();
    miniH.pop();

    dijkstra(ajdlist, src.node, ans, miniH);

    for (uint i = 1; i < ans.size(); i++) {
        if (ans[i] == 1e8 + 7)
            cout << -1 << " ";
        else
            cout << ans[i] << " ";
    }
    cout << endl;
}

int main() {
    int nodes, edges, noofstations;
    cin >> nodes >> edges >> noofstations;

    std::vector<List *> adjlist(nodes + 1);
    std::vector<int> policestations;

    for (uint i = 1; i < nodes + 1; i++) {
        adjlist[i] = new List();
    }

    while (edges--) {
        int u, v, w;
        cin >> u >> v >> w;
        Node *e = new Node(v, w);
        adjlist[u]->add(e);
        Node *_e = new Node(u, w);
        adjlist[v]->add(_e);
    }

    while (noofstations--) {
        int k;
        cin >> k;
        policestations.push_back(k);
    }

    nearest_station(adjlist, policestations);
}

/*
11 9 3
1 5 10
2 6 10
3 7 10
5 6 30
5 8 10
6 9 5
7 11 10
8 9 10
9 10 15
3 5 10
 */