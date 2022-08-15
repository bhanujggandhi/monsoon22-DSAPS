#include <iostream>

using namespace std;

template <typename T>

class Deque
{
    T *arr;
    int curr_size;
    int max_size;
    int _front;
    int _rear;

public:
    Deque()
    {
        curr_size = 0;
        max_size = 2;
        _front = -1;
        _rear = 3;
        arr = new T[max_size];
    }

    Deque(const int n, const T x)
    {
        curr_size = n;
        max_size = n;
        _front = n / 2;
        _rear = n / 2 + 1;
        arr = new T[max_size];

        for (int i = 0; i < n; i++)
        {
            arr[i] = x;
        }
    }

    void push_back(const T val)
    {
        if (curr_size == max_size or _rear = _front + 1)
        {
            // Double the size and copy
            T *oldarr = arr;
            max_size = 2 * max_size;
            arr = new T[max_size];

            for (int i = 0; i < curr_size; i++)
            {
                arr[i] = oldarr[i];
            }

            delete[] oldarr;
        }
        arr[--_rear] = val;
        curr_size++;
    }

    void pop_back()
    {
        if (_rear != max_size + 1)
        {
            _rear++;
        }
    }

    void push_front(const T val)
    {
        if (_front == -1)
        {
            _front = _rear = 0;
            arr[_front] = val;
        }

        if ((_front - 1) % max_size == _rear)
        {
            int *oldarr = arr;
            int oldms = max_size;
            max_size = max_size * 2;
            arr = new int[max_size];

            if (_rear < _front)
            {
                int i = 0;

                for (i = 0; i <= _rear; i++)
                {
                    arr[i] = oldarr[i];
                }
                _rear = i - 1;
                i = oldms - 1;
                int k = max_size - 1;

                for (; i >= _front; i--)
                {
                    arr[k--] = oldarr[i];
                }
                _front = k + 1;
            }
            else
            {
                int k = 0;
                for (int i = _front; i <= _rear; i++)
                {
                    arr[k++] = oldarr[i];
                }
                _front = 0;
                _rear = k - 1;
            }
        }

        _front = (_front + 1) % max_size;
        arr[front] = val;
    }

    void pop_front()
    {
        if (_front == _rear)
        {
            _front = _rear = -1;
        }
        else if (_front == -1)
        {
            throw underflow_error("Deque is empty");
        }
        else
        {
            _front = (_front + 1) % max_size;
        }
    }

    bool empty() const
    {
        return curr_size == 0;
    }

    T front() const
    {
        return arr[_front];
    }

    T back() const
    {
        return arr[_rear];
    }

    T at(int i) const
    {
        if (i >= 0 and i < curr_size)
            return arr[i];
        else
            throw out_of_range("Index is out of bounds");
    }

    int size() const
    {
        return curr_size;
    }

    int capacity() const
    {
        return max_size;
    }

    T operator[](const int i) const
    {
        if (i >= 0 and i < curr_size)
            return arr[i];
        else
            throw out_of_range("Index is out of bounds");
    }

    void resize(int x, T d) {}

    void clear()
    {
        int *oldarr = arr;
        curr_size = 0;
        max_size = 1;
        delete[] oldarr;
        arr = new int[max_size];
    }
};

int main()
{
    Deque<int> dq;

    dq.push_back(3);
    dq.push_back(4);
    cout << dq.capacity() << endl;
    cout << dq.size() << endl;
    dq.push_back(4);
    cout << dq.capacity() << endl;
    cout << dq.size() << endl;

    // dq.pop_back();
    // dq.pop_back();
    // cout << dq.front() << endl;
    // cout << dq.back() << endl;

    // cout << dq[3] << endl;
}