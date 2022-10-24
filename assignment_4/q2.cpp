#include <iostream>
#include <list>

using std::cout, std::endl;

struct Node {
    int key;
    int value;
    Node *prev;
    Node *next;

    Node(int _key, int _value) {
        key = _key;
        value = _value;
        prev = NULL;
        next = NULL;
    }
};

struct List {
    Node *head;
    Node *tail;
    int size;

    List() {
        size = 0;
        head = new Node(-1, -1);
        tail = new Node(-1, -1);
        head->next = tail;
        tail->prev = head;
    }

    ~List() {
        Node *temp;
        while (head != NULL) {
            temp = head;
            delete temp;
            head = head->next;
        }
    }

    void add(Node *data) {
        head->next->prev = data;
        data->next = head->next;
        head->next = data;
        data->prev = head;
        this->size++;
    }

    void remove(Node *data) {
        data->prev->next = data->next;
        data->next->prev = data->prev;
        this->size--;
    }
};

class unordered_map {
   private:
    /* Data */
    List *buckets;
    int hash_table_size;
    int total_elements;
    float load_factor;

    /* Methods */
    int hash_function(int key) { return key % hash_table_size; }

   public:
    unordered_map(/* args */);
    ~unordered_map();
    Node *map(int key);
    bool find(int key);
    void insert(int key, int value);
    void rehash();
    void erase(int key);
};

unordered_map::unordered_map() {
    hash_table_size = 8;
    buckets = new List[hash_table_size];
    total_elements = 0;
    load_factor = 1;
}

unordered_map::~unordered_map() { delete[] buckets; }

Node *unordered_map::map(int key) {
    for (auto it = buckets[hash_function(key)].head->next;
         it != buckets[hash_function(key)].tail; it = it->next) {
        if (it->key == key) {
            return it;
        }
    }
    return buckets[hash_function(key)].tail;
}

bool unordered_map::find(int key) {
    for (auto it = buckets[hash_function(key)].head->next;
         it != buckets[hash_function(key)].tail; it = it->next) {
        if (it->key == key) {
            return true;
        }
    }
    return false;
}

void unordered_map::insert(int key, int value) {
    auto it = map(key);
    if (it != buckets[hash_function(key)].tail) {
        it->value = value;
        return;
    }

    buckets[hash_function(key)].add(new Node(key, value));
    total_elements++;
    rehash();
}

void unordered_map::erase(int key) {
    auto it = map(key);

    if (it != buckets[hash_function(key)].tail) {
        buckets[hash_function(key)].remove(it);
        total_elements--;
    }
}

void unordered_map::rehash() {
    float lf = (float)total_elements / hash_table_size;
    if (lf <= load_factor) {
        return;
    }

    hash_table_size = hash_table_size * 2;

    auto new_buckets = new List[hash_table_size];
    for (int i = 0; i < hash_table_size / 2; i++) {
        for (auto it = buckets[i].head->next; it != buckets[i].tail;
             it = it->next) {
            new_buckets[hash_function(it->key)].add(
                new Node(it->key, it->value));
        }
    }

    delete[] buckets;
    buckets = new_buckets;
}

int main() {
    unordered_map map;

    map.insert(1, 2);
    map.insert(1, 3);
    map.insert(1, 4);
    map.insert(2, 2);
    map.insert(3, 2);
    map.insert(5, 7);
    map.insert(7, 2);
    map.insert(9, 2);
    map.insert(31, 2);
    map.insert(15, 9);
    map.insert(16, 12);
    map.insert(48, 29);
    map.insert(64, 29);
    map.insert(32, 19);
    auto it = map.map(5);
    cout << it->value << endl;
    map.erase(5);
    it = map.map(5);
    cout << it->value << endl;

    cout << map.find(9) << endl;

    return 0;
}