#include <iostream>

using std::cout, std::endl;

/// @brief Node structure which defines an entity of List stucture
/// @tparam T1
/// @tparam T2
template <class T1, class T2>
struct Node {
    T1 key;
    T2 value;
    Node<T1, T2> *prev;
    Node<T1, T2> *next;

    Node(T1 _key, T2 _value) {
        key = _key;
        value = _value;
        prev = NULL;
        next = NULL;
    }
};

/// @brief Linked List implementation which inserts at the front and removes the
/// desired node
/// @tparam T1
/// @tparam T2
template <class T1, class T2>
struct List {
    Node<T1, T2> *head;
    Node<T1, T2> *tail;
    int size;

    List() {
        size = 0;
        head = new Node<T1, T2>(T1(), T2());
        tail = new Node<T1, T2>(T1(), T2());
        head->next = tail;
        tail->prev = head;
    }

    ~List() {
        Node<T1, T2> *temp;
        while (head != NULL) {
            temp = head;
            delete temp;
            head = head->next;
        }
    }

    void add(Node<T1, T2> *data) {
        head->next->prev = data;
        data->next = head->next;
        head->next = data;
        data->prev = head;
        this->size++;
    }

    void remove(Node<T1, T2> *data) {
        data->prev->next = data->next;
        data->next->prev = data->prev;
        this->size--;
    }
};

/// @brief unordered_map class which hashes keys to some bucket. Insert, erase,
/// find, map implemented
/// @tparam T1
/// @tparam T2
template <class T1, class T2>
class unordered_map {
   private:
    /* Data */
    List<T1, T2> *buckets;
    int hash_table_size;
    int total_elements;
    float load_factor;

    /* Methods */

    /// @brief Hash function which maps a certain key to a bucket in the
    /// unordered_map
    /// @param key
    /// @return integer value to map at the bucket[i]
    int hash_function(T1 key) { return key % hash_table_size; }

    /// @brief Function to increase the bucket size of the map if full.
    void rehash() {
        float lf = (float)total_elements / hash_table_size;
        if (lf <= load_factor) {
            return;
        }

        hash_table_size = hash_table_size * 2;

        auto new_buckets = new List<T1, T2>[hash_table_size];
        for (int i = 0; i < hash_table_size / 2; i++) {
            for (auto it = buckets[i].head->next; it != buckets[i].tail;
                 it = it->next) {
                new_buckets[hash_function(it->key)].add(
                    new Node<T1, T2>(it->key, it->value));
            }
        }

        delete[] buckets;
        buckets = new_buckets;
    }

   public:
    /// @brief unordered_map intializer
    unordered_map() {
        hash_table_size = 8;
        buckets = new List<T1, T2>[hash_table_size];
        total_elements = 0;
        load_factor = 1;
    }

    ~unordered_map() { delete[] buckets; }

    /// @brief Finds and returns the value mapped to the key provided as param
    /// @param key
    /// @return Node pointer if found, else the tail pointer of the list
    Node<T1, T2> *map(T1 key) {
        for (auto it = buckets[hash_function(key)].head->next;
             it != buckets[hash_function(key)].tail; it = it->next) {
            if (it->key == key) {
                return it;
            }
        }
        return buckets[hash_function(key)].tail;
    }

    /// @brief Finds and returns true or false if the key is mapped to some
    /// value in the map
    /// @param key
    /// @return 0 if not found, 1 if found
    bool find(T1 key) {
        for (auto it = buckets[hash_function(key)].head->next;
             it != buckets[hash_function(key)].tail; it = it->next) {
            if (it->key == key) {
                return true;
            }
        }
        return false;
    }

    /// @brief Insert the key, value mapping to the bucket
    /// @param key
    /// @param value
    void insert(T1 key, T2 value) {
        Node<T1, T2> *it = map(key);
        if (it != buckets[hash_function(key)].tail) {
            it->value = value;
            return;
        }

        buckets[hash_function(key)].add(new Node<T1, T2>(key, value));
        total_elements++;
        rehash();
    }

    /// @brief Removes the particular key value pair if present in the map
    /// @param key
    void erase(T1 key) {
        auto it = map(key);

        if (it != buckets[hash_function(key)].tail) {
            buckets[hash_function(key)].remove(it);
            total_elements--;
        }
    }
};

int main() {
    unordered_map<int, std::string> map;

    map.insert(1, "hello");
    map.insert(1, "helo");
    map.insert(1, "hell");
    map.insert(2, "hllo");
    map.insert(3, "hel");
    map.insert(5, "ello");
    map.insert(7, "hllo");
    map.insert(9, "heo");
    map.insert(31, "ello");
    map.insert(15, "llo");
    map.insert(16, "lo");
    map.insert(48, "o");
    map.insert(64, "l");
    map.insert(32, "w");
    auto it = map.map(32);
    cout << it->value << endl;
    map.erase(5);
    it = map.map(5);
    cout << it->value << endl;

    cout << map.find(32) << endl;

    return 0;
}