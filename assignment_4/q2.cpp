#include <iostream>
#include <list>

using std::cout, std::endl;

class unordered_map {
   private:
    /* Data */
    std::list<std::pair<int, int>> *buckets;
    int hash_table_size;
    int total_elements;
    float load_factor;

    /* Methods */
    int hash_function(int key) { return key % hash_table_size; }

   public:
    unordered_map(/* args */);
    ~unordered_map();
    std::list<std::pair<int, int>>::iterator map(int key);
    bool find(int key);
    void insert(int key, int value);
    void rehash();
    void erase(int key);
};

unordered_map::unordered_map() {
    hash_table_size = 8;
    buckets = new std::list<std::pair<int, int>>[hash_table_size];
    total_elements = 0;
    load_factor = 1;
}

unordered_map::~unordered_map() {}

std::list<std::pair<int, int>>::iterator unordered_map::map(int key) {
    for (auto it = buckets[hash_function(key)].begin();
         it != buckets[hash_function(key)].end(); it++) {
        if (it->first == key) {
            return it;
        }
    }
    return buckets[hash_function(key)].end();
}

bool unordered_map::find(int key) {
    for (auto it = buckets[hash_function(key)].begin();
         it != buckets[hash_function(key)].end(); it++) {
        if (it->first == key) {
            return true;
        }
    }
    return false;
}

void unordered_map::insert(int key, int value) {
    auto it = map(key);
    if (it != buckets[hash_function(key)].end()) {
        it->second = value;
        return;
    }

    buckets[hash_function(key)].push_back({key, value});
    total_elements++;
    rehash();
}

void unordered_map::erase(int key) {
    auto it = map(key);

    if (it != buckets[hash_function(key)].end()) {
        buckets[hash_function(key)].erase(it);
        total_elements--;
    }
}

void unordered_map::rehash() {
    float lf = (float)total_elements / hash_table_size;
    if (lf <= load_factor) {
        return;
    }

    hash_table_size = hash_table_size * 2;

    auto new_buckets = new std::list<std::pair<int, int>>[hash_table_size];
    for (int i = 0; i < hash_table_size / 2; i++) {
        for (auto it = buckets[i].begin(); it != buckets[i].end(); it++) {
            new_buckets[hash_function(it->first)].push_back(
                {it->first, it->second});
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
    map.insert(21, 3);
    map.insert(4, 32);
    map.insert(94, 31);
    map.insert(494, 32);
    map.insert(834, 1);
    auto it = map.map(5);
    cout << it->second << endl;
    map.erase(5);
    it = map.map(5);
    cout << it->second << endl;

    cout << map.find(9) << endl;

    return 0;
}