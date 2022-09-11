#include <iostream>
#include <unordered_map>

struct Node
{
    int key;
    int value;
    int frequency;
    Node *prev;
    Node *next;

    Node(int _key, int _value)
    {
        key = _key;
        value = _value;
        frequency = 0;
        prev = NULL;
        next = NULL;
    }
};

struct List
{
    Node *front;
    Node *tail;
    int size;

    List()
    {
        size = 0;
        front = new Node(-1, -1);
        tail = new Node(-1, -1);
        front->next = tail;
        tail->prev = front;
    }

    void add(Node *data)
    {
        front->next->prev = data;
        data->next = front->next;
        front->next = data;
        data->prev = front;
        size++;
    }

    void remove(Node *data)
    {
        data->prev->next = data->next;
        data->next->prev = data->prev;
        delete data;
        size--;
    }
};

class LFUCache
{
private:
    std::unordered_map<int, Node *> keynodelogger;
    std::unordered_map<int, List *> freqlistlogger;
    int capacity;
    int leastFrequency;

public:
    LFUCache(int _capacity);
    ~LFUCache();
    void set(int key, int value);
};

LFUCache::LFUCache(int _capacity)
{
    keynodelogger.clear();
    freqlistlogger.clear();
    capacity = _capacity;
    leastFrequency = 0;
}

LFUCache::~LFUCache()
{
}

void LFUCache::set(int key, int value)
{
    // Case 1: If the key is already present
    if (keynodelogger.find(key) != keynodelogger.end())
    {
        Node *old = keynodelogger[key];

        // Update the value
        old->value = value;

        // Remove from the current frequency list
        List *oldlist = freqlistlogger[old->frequency];
        oldlist->remove(old);

        // Check if oldnode has the least freq and oldlist is empty
        if (old->frequency == leastFrequency and oldlist->size == 0)
            leastFrequency++;

        List *newlist;
        // If current node frequency + 1 has already a list
        if (freqlistlogger.find(old->frequency + 1) != freqlistlogger.end())
        {
            newlist = freqlistlogger[old->frequency + 1];
            old->frequency += 1;
            newlist->add(old);
        }
        // If no list, then create one
        else
        {
            newlist = new List();
            old->frequency += 1;
            newlist->add(old);
            freqlistlogger.insert({old->frequency, newlist});
        }
    }
    // Case 2: If the node is not already present
    else
    {
        // If capacity is full
        if (keynodelogger.size() == capacity)
        {
            List *minlist = freqlistlogger[leastFrequency];
            // Remove from the tail node
            Node *removenode = minlist->tail->prev;
            int minkey = removenode->key;
            minlist->remove(removenode);
            keynodelogger.erase(minkey);
        }

        List *newlist;
        // This new node is first time accessed
        leastFrequency = 1;
        if (freqlistlogger.find(leastFrequency) != freqlistlogger.end())
        {
            newlist = freqlistlogger[leastFrequency];
            Node *newnode = new Node(key, value);

            newlist->add(newnode);
            keynodelogger.insert({key, newnode});
        }
        else
        {
            newlist = new List();
            Node *newnode = new Node(key, value);

            newlist->add(newnode);
            keynodelogger.insert({key, newnode});
            freqlistlogger.insert({leastFrequency, newlist});
        }
    }
}

int main()
{

    return 0;
}