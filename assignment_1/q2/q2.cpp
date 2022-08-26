#include <iostream>

using namespace std;

template <typename T>

class Deque
{
    T *arr;
    int max_size;
    int _front;
    int _rear;

public:
    void deque()
    {
        max_size = 2;
        _front = -1;
        _rear = -1;
        arr = new T[max_size];
    }

    void deque(const int n, const T x)
    {
        max_size = n;
        _front = 0;
        _rear = n - 1;
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
            T *oldarr = arr;
            int oldms = max_size;
            max_size = max_size * 2;
            arr = new T[max_size];

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
            cout << "underflow_error: Deque is empty" << endl;
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
            T *oldarr = arr;
            int oldms = max_size;
            max_size = max_size * 2;
            arr = new T[max_size];

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
            cout << "underflow_error: Deque is empty" << endl;
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
            return T();
    }

    T back() const
    {
        if (_rear != -1)
            return arr[_rear];
        else
            return T();
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
            return T();

        if (i >= size())
            return T();

        if (_front + i >= max_size)
        {
            return arr[_front - max_size + i];
        }
        else
        {
            return arr[_front + i];
        }
    }

    void resize(int newsize, T data)
    {
        int oldsize = size();

        T *oldarr = arr;
        arr = new T[newsize];
        int oldms = max_size;
        max_size = newsize;

        // newsize > oldsize
        if (newsize > oldsize)
        {
            if (_rear > _front)
            {
                int temp = 0;
                for (int i = _front; i <= _rear; i++)
                {
                    arr[temp++] = oldarr[i];
                }
                cout << temp << "    " << newsize << endl;

                while (temp < newsize)
                {
                    arr[temp] = data;
                    temp++;
                }

                _front = 0;
                _rear = newsize - 1;
            }
            else
            {
                for (int i = 0; i < newsize; i++)
                {
                    arr[i] = data;
                }
                int k = 0;
                int i = _front;
                int count = 0;

                for (; i <= oldms - 1; i++)
                {
                    if (count == oldsize)
                        break;
                    arr[k++] = oldarr[i];
                    count++;
                }
                _front = 0;
                if (count == oldsize)
                    _rear = newsize - 1;
                else
                {
                    i = 0;
                    for (; i <= _rear; i++)
                    {
                        if (count == newsize)
                            break;
                        arr[k++] = oldarr[i];
                        count++;
                    }
                    _rear = newsize - 1;
                }
            }
        }

        // newsize < oldsize
        else
        {
            if (_rear > _front)
            {
                int k = 0;
                for (int i = _front; i < newsize; i++)
                {
                    arr[k++] = oldarr[i];
                }
                _front = 0;
                _rear = newsize - 1;
            }
            else
            {
                int k = 0;
                int i = _front;
                int count = 0;

                for (; i <= oldms - 1; i++)
                {
                    if (count == newsize)
                        break;
                    arr[k++] = oldarr[i];
                    count++;
                }
                _front = 0;
                if (count == newsize)
                    _rear = k - 1;
                else
                {
                    i = 0;
                    for (; i <= _rear; i++)
                    {
                        if (count == newsize)
                            break;
                        arr[k++] = oldarr[i];
                        count++;
                    }
                    _rear = k - 1;
                }
            }
        }
        delete[] oldarr;
    }

    void clear()
    {
        T *oldarr = arr;
        _front = _rear = -1;
        max_size = 2;
        delete[] oldarr;
        arr = new T[max_size];
    }
};

int main()
{
    Deque<string> DQ; // or Deque<float> DQ;
    string val;       // or float val;
    // 0 - QUIT
    // 1 - deque()
    // 2 - deque(n, x)
    // 3 - push_back(x)
    // 4 - pop_back()
    // 5 - push_front(x)
    // 6 - pop_front()
    // 7 - front()
    // 8 - back()
    // 9 - empty()
    // 10 - size()
    // 11 - resize(n, x)
    // 12 - clear()
    // 13 - D[n]
    // 14 - display()
    // 15 - capacity()
    int choice, flag = 1;
    int n;

    while (flag)
    {
        cout << "-------------------------\n";
        cout << "choice: ";
        cin >> choice;
        switch (choice)
        {
        case 0:
            flag = 0;
            break;
        case 1:
            DQ.deque();
            break;
        case 2:
            cin >> n >> val;
            DQ.deque(n, val);
            break;
        case 3:
            cin >> val;
            DQ.push_back(val);
            break;
        case 4:
            DQ.pop_back();
            break;
        case 5:
            cin >> val;
            DQ.push_front(val);
            break;
        case 6:
            DQ.pop_front();
            break;
        case 7:
            cout << DQ.front() << endl;
            break;
        case 8:
            cout << DQ.back() << endl;
            break;
        case 9:
            cout << ((DQ.empty()) ? "true" : "false") << endl;
            break;
        case 10:
            cout << DQ.size() << endl;
            break;
        case 11:
            cin >> n >> val;
            DQ.resize(n, val);
            break;
        case 12:
            DQ.clear();
            break;
        case 13:
            cin >> n;
            cout << DQ[n] << endl;
            break;
        case 14:
            // For Debugging
            for (int i = 0; i < DQ.size(); i++)
                cout << DQ[i] << "  ";
            cout << endl;
            break;
        case 15:
            // For Debugging
            cout << DQ.capacity() << endl;
            break;
        default:
            cout << "Enter correct choice\n";
        }
    }
    return 0;
}
// 0 - QUIT
// 1 - deque()
// 2 - deque(n, x)
// 3 - push_back(x)
// 4 - pop_back()
// 5 - push_front(x)
// 6 - pop_front()
// 7 - front()
// 8 - back()
// 9 - empty()
// 10 - size()
// 11 - resize(n, x)
// 12 - clear()
// 13 - D[n]
// 14 - display()
// 15 - capacity()

// Deque<int> dq;
// dq.push_back(1);
// dq.push_front(2);
// dq.push_front(3);
// dq.push_back(4);
// dq.push_back(5);

// cout << dq[0] << endl;

// cout << "SIZE: " << dq.size() << endl;
// cout << "FRONT ELEMENT: " << dq.front() << endl;
// cout << "REAR ELEMENT: " << dq.back() << endl;

// dq.resize(2, 100);

// for (int i = 0; i < dq.size(); i++)
// {
//     cout << dq[i] << "    ";
// }
// cout << endl;
// cout << "FRONT ELEMENT: " << dq.front() << endl;
// cout << "REAR ELEMENT: " << dq.back() << endl;

// Deque<char> dq2(10, 'a');
// cout << dq.front() << endl;
// cout << dq.back() << endl;

// dq2.push_back('b');
// dq2.push_back('c');
// dq2.push_back('d');
// dq2.push_back('e');
// dq2.push_back('f');

// cout << "SIZE: " << dq2.size() << endl;

// cout << "FRONT ELEMENT: " << dq2.front() << endl;
// cout << "REAR ELEMENT: " << dq2.back() << endl;

// dq2.resize(20, 'k');

// for (int i = 0; i < dq2.size(); i++)
// {
//     cout << dq2[i] << "    ";
// }
// cout << endl;

// Deque<string> d3;
// d3.push_back("Hello");
// cout << d3.front() << endl;
// cout << d3.back() << endl;
// d3.pop_front();
// cout << d3.front() << endl;
// cout << d3.back() << endl;
