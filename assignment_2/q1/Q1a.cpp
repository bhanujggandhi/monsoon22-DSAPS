#include <iostream>
#include <unordered_map>

// Structure of Node for doubly linked list
struct Node
{
    int key;
    int value;
    Node *prev;
    Node *next;

    Node(int key, int value)
    {
        this->key = key;
        this->value = value;
        this->prev = NULL;
        this->next = NULL;
    }
};

// LRUCache Class Declaration
class LRUCache
{
private:
    int capacity;
    Node *front;
    Node *tail;
    std::unordered_map<int, Node *> logger;

public:
    LRUCache(int _capacity);
    ~LRUCache();
    void set(int key, int value);
    int get(int key);
    void display();
};

// Contructor to set the class variables
LRUCache::LRUCache(const int _capacity)
{
    capacity = _capacity;
    front = NULL;
    tail = NULL;
    logger.clear();
}

// Destructor to clean the memory
LRUCache::~LRUCache()
{
    Node *temp = front;
    while (front != NULL)
    {
        front = front->next;
        delete temp;
        temp = front;
    }
}

// Function to set key, value in the cache class
void LRUCache::set(int key, int value)
{
    // 1. Key is not present and capacity is not full
    if (logger.size() < capacity and logger.find(key) == logger.end())
    {
        Node *curr = new Node(key, value);
        // First element in the cache
        if (logger.size() == 0)
        {
            front = curr;
            tail = curr;
        }
        else
        {
            curr->next = front;
            front->prev = curr;
            front = front->prev;
        }
        logger.insert({key, curr});
    }
    // 2. Key is not present and capacity is full
    else if (logger.size() == capacity and logger.find(key) == logger.end())
    {
        // Add the new key to the list
        Node *curr = new Node(key, value);
        curr->next = front;
        front->prev = curr;
        front = front->prev;

        // Remove the least recently used key
        Node *temp = tail->prev;
        // This is the only node in the list (When capacity = 1)
        if (temp == NULL)
        {
            front = NULL;
            logger.erase(tail->key);
            delete tail;
            tail = NULL;
        }
        else
        {
            temp->next = NULL;
            logger.erase(tail->key);
            delete tail;
            tail = temp;
        }
        logger.insert({key, curr});
    }
    // If key is already present in the map
    else if (logger.find(key) != logger.end())
    {
        Node *old = logger[key];
        // It could be a update request
        Node *curr = new Node({key, value});

        // If this is the only node
        if (old->prev == NULL and old->next == NULL)
        {
            front = tail = curr;
            logger.erase(key);
            logger.insert({key, curr});
            delete old;
        }
        else
        {
            // If old is the front itself
            if (old->prev == NULL)
            {
                front = front->next;
                front->prev = NULL;
                logger.erase(key);
                delete old;
            }
            // If old is the tail
            else if (old->next == NULL)
            {
                tail = tail->prev;
                old->prev->next = NULL;
                logger.erase(key);
                delete old;
            }
            // Remove the old from the middle
            else
            {
                old->prev->next = old->next;
                old->next->prev = old->prev;
                logger.erase(key);
                delete old;
            }

            // Insert current at the front
            curr->next = front;
            front->prev = curr;
            front = front->prev;
            logger.insert({key, curr});
        }
    }
}

int LRUCache::get(int key)
{
    if (logger.size() != 0 and logger.find(key) != logger.end())
    {
        Node *old = logger[key];

        // If old is the front itself
        if (old->prev == NULL)
        {
            return old->value;
        }
        // If old is at the end
        else if (old->next == NULL)
        {
            tail = tail->prev;
            old->prev->next = NULL;
            old->prev = NULL;
        }
        // Remove old from the middle
        else
        {
            old->prev->next = old->next;
            old->next->prev = old->prev;
            old->prev = NULL;
            old->next = NULL;
        }

        // Pushing old to the front
        old->next = front;
        front->prev = old;
        front = front->prev;

        return old->value;
    }
    else
    {
        return -1;
    }
}

void LRUCache::display()
{
    Node *temp = front;

    while (temp != NULL)
    {
        std::cout << "(" << temp->key << ", " << temp->value << ")" << std::endl;
        temp = temp->next;
    }
}

int main()
{
    LRUCache c(2);
    c.set(2, 1);
    c.set(2, 2);
    std::cout << c.get(2) << std::endl;
    c.set(1, 1);
    c.set(4, 1);
    std::cout << c.get(2) << std::endl;

    /*
    ["LRUCache","put","put","get","put","put","get"]
    [[2],[2,1],[2,2],[2],[1,1],[4,1],[2]]
    */

    return 0;
}