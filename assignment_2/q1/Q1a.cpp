#include <iostream>
#include <unordered_map>

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

LRUCache::LRUCache(const int _capacity)
{
    capacity = _capacity;
    front = NULL;
    tail = NULL;
    logger.clear();
}

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

void LRUCache::set(int key, int value)
{
    // 1. Key is not present and capacity is not full
    if (logger.size() < capacity and logger.find(key) == logger.end())
    {
        Node *curr = new Node(key, value);
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
        // This is the only node in the list
        if (temp == NULL)
        {
            front = NULL;
            tail = NULL;
            delete tail;
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
    else if (logger.find(key) != logger.end())
    {
        Node *old = logger[key];
        Node *curr = new Node({key, value});
        // If old is the front itself
        if (old->prev == NULL)
        {
            front = front->next;
            front->prev = NULL;
            delete old;
        }
        else if (old->next == NULL)
        {
            old->prev->next = NULL;
            delete old;
        }
        else
        {
            old->prev->next = old->next;
            old->next->prev = old->prev;
            delete old;
        }

        curr->next = front;
        front->prev = curr;
        front = front->prev;
        logger[key] = curr;
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
            old->prev->next = NULL;
            old->prev = NULL;
        }
        else
        {
            old->prev->next = old->next;
            old->next->prev = old->prev;
            old->prev = NULL;
            old->next = NULL;
        }

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
    LRUCache c(3);
    c.set(1, 2);
    c.set(2, 3);
    c.set(3, 4);
    c.display();
    std::cout << "----------" << std::endl;
    c.set(4, 5);
    c.display();
    std::cout << "----------" << std::endl;
    c.set(2, 6);
    c.display();
    std::cout << "----------" << std::endl;
    c.set(2, 1);
    c.display();
    std::cout << "----------" << std::endl;
    c.set(4, 14);
    c.display();
    std::cout << "----------" << std::endl;
    std::cout << c.get(3) << std::endl;
    c.display();
    std::cout << "----------" << std::endl;
    std::cout << c.get(4) << std::endl;
    c.display();
    std::cout << "----------" << std::endl;
    std::cout << c.get(2) << std::endl;
    c.display();

    return 0;
}