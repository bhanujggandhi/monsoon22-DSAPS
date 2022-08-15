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
        if (_front == -1)
        {
            _front = _rear = 0;
            arr[_front] = val;
            return;
        }

        if ((_rear + 1) % max_size == _front)
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

        _rear = (_rear + 1) % max_size;
        arr[_rear] = val;
    }

    void pop_back()
    {
        if (_front == _rear and _rear != -1)
        {
            _front = _rear = -1;
        }
        else if (_rear == -1)
        {
            throw underflow_error("Deque is empty");
        }
        else
        {
            if (_rear - 1 < 0)
                _rear = (_rear + max_size - 1) % max_size;
            else
                _rear = (_rear - 1) % max_size;
        }
    }

    void push_front(const T val)
    {
        if (_front == -1)
        {
            _front = _rear = 0;
            arr[_front] = val;
            return;
        }

        bool flag = false;
        if (_front - 1 < 0)
        {
            flag = ((_front - 1 + max_size) % max_size == _rear);
        }
        else
            flag = (_front - 1) % max_size == _rear;
        if (flag)
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

        if (_front - 1 < 0)
            _front = (_front + max_size - 1) % max_size;
        else
            _front = (_front - 1) % max_size;
        arr[_front] = val;
        // cout << "FRONT: " << _front << endl;
    }

    void pop_front()
    {
        if (_front == _rear and _front != -1)
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
        return _front == -1;
    }

    T front() const
    {
        if (_front != -1)
            return arr[_front];
        else
            cout << "Deque is empty" << endl;
        return INT32_MAX;
    }

    T back() const
    {
        if (_rear != -1)
            return arr[_rear];
        else
            cout << "Deque is empty" << endl;
        return INT32_MAX;
    }

    T at(int i) const
    {
        if (_front == -1)
        {
            cout << "Out of bounds" << endl;
            return -10;
        }

        if (_front + i >= max_size)
        {
            return arr[_front - max_size + i];
        }
        else
        {
            return arr[_front + i];
        }
    }

    int size() const
    {
        if (_front == _rear and _front == -1)
        {
            return 0;
        }
        if (_front < _rear)
            return (_rear - _front + 1);
        else if (_front == _rear)
        {
            return 1;
        }
        else
            return max_size - _front + _rear + 1;
    }

    int capacity() const
    {
        return max_size;
    }

    T operator[](const int i) const
    {
        if (_front == -1)
        {
            cout << "Out of bounds" << endl;
            return -10;
        }

        if (_front + i >= max_size)
        {
            return arr[_front - max_size + i];
        }
        else
        {
            return arr[_front + i];
        }
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

    dq.push_back(1);
    dq.push_front(2);
    dq.push_front(3);
    dq.push_back(4);
    dq.push_back(5);

    cout << "SIZE: " << dq.size() << endl;
    cout << "FRONT ELEMENT: " << dq.front() << endl;
    cout << "REAR ELEMENT: " << dq.back() << endl;

    for (int i = 0; i < dq.size(); i++)
    {
        cout << dq[i] << "    ";
    }
    cout << endl;

    Deque<int> dq2;

    dq2.push_back(1);
    dq2.push_back(2);
    dq2.push_back(3);
    dq2.push_back(4);
    dq2.push_back(5);
    dq2.pop_back();
    dq2.pop_back();
    dq2.pop_front();
    dq2.pop_front();
    dq2.pop_front();

    cout << "SIZE: " << dq2.size() << endl;

    cout << "FRONT ELEMENT: " << dq2.front() << endl;
    cout << "REAR ELEMENT: " << dq2.back() << endl;

    for (int i = 0; i < dq2.size(); i++)
    {
        cout << dq2[i] << "    ";
    }
    cout << endl;
}