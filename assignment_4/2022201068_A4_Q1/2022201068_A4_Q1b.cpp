#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;

#define ALPHABET 256;

struct Pair {
    int first;
    int second;

    Pair(int _f, int _s) {
        first = _f;
        second = _s;
    }
};

template <typename T>
class Deque {
    T *arr;
    int max_size;
    int _front;
    int _rear;

   public:
    void deque() {
        max_size = 1000;
        _front = -1;
        _rear = -1;
        arr = new T[max_size];
    }

    void deque(const int n, const T x) {
        max_size = n;
        _front = 0;
        _rear = n - 1;
        arr = new T[max_size];

        for (int i = 0; i < n; i++) {
            arr[i] = x;
        }
    }

    void push_back(const T val) {
        if (_front == -1) {
            _front = _rear = 0;
            arr[_front] = val;
            return;
        }

        if ((_rear + 1) % max_size == _front) {
            T *oldarr = arr;
            int oldms = max_size;
            max_size = max_size * 2;
            arr = new T[max_size];

            if (_rear < _front) {
                int i = 0;

                for (i = 0; i <= _rear; i++) {
                    arr[i] = oldarr[i];
                }
                _rear = i - 1;
                i = oldms - 1;
                int k = max_size - 1;

                for (; i >= _front; i--) {
                    arr[k--] = oldarr[i];
                }
                _front = k + 1;
            } else {
                int k = 0;
                for (int i = _front; i <= _rear; i++) {
                    arr[k++] = oldarr[i];
                }
                _front = 0;
                _rear = k - 1;
            }
        }

        _rear = (_rear + 1) % max_size;
        arr[_rear] = val;
    }

    void pop_back() {
        if (_front == _rear and _rear != -1) {
            _front = _rear = -1;
        } else if (_rear == -1) {
            cout << "underflow_error: Deque is empty" << endl;
        } else {
            if (_rear - 1 < 0)
                _rear = (_rear + max_size - 1) % max_size;
            else
                _rear = (_rear - 1) % max_size;
        }
    }

    void push_front(const T val) {
        if (_front == -1) {
            _front = _rear = 0;
            arr[_front] = val;
            return;
        }

        bool flag = false;
        if (_front - 1 < 0) {
            flag = ((_front - 1 + max_size) % max_size == _rear);
        } else
            flag = (_front - 1) % max_size == _rear;
        if (flag) {
            T *oldarr = arr;
            int oldms = max_size;
            max_size = max_size * 2;
            arr = new T[max_size];

            if (_rear < _front) {
                int i = 0;

                for (i = 0; i <= _rear; i++) {
                    arr[i] = oldarr[i];
                }
                _rear = i - 1;
                i = oldms - 1;
                int k = max_size - 1;

                for (; i >= _front; i--) {
                    arr[k--] = oldarr[i];
                }
                _front = k + 1;
            } else {
                int k = 0;
                for (int i = _front; i <= _rear; i++) {
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
    }

    void pop_front() {
        if (_front == _rear and _front != -1) {
            _front = _rear = -1;
        } else if (_front == -1) {
            cout << "underflow_error: Deque is empty" << endl;
        } else {
            _front = (_front + 1) % max_size;
        }
    }

    bool empty() const {
        return _front == -1;
    }

    T front() const {
        if (_front != -1)
            return arr[_front];
        else
            return T();
    }

    T back() const {
        if (_rear != -1)
            return arr[_rear];
        else
            return T();
    }

    int size() const {
        if (_front == _rear and _front == -1) {
            return 0;
        }
        if (_front < _rear)
            return (_rear - _front + 1);
        else if (_front == _rear) {
            return 1;
        } else
            return max_size - _front + _rear + 1;
    }

    int capacity() const {
        return max_size;
    }

    T operator[](const int i) const {
        if (_front == -1)
            return T();

        if (i >= size())
            return T();

        if (_front + i >= max_size) {
            return arr[_front - max_size + i];
        } else {
            return arr[_front + i];
        }
    }

    void resize(int newsize, T data) {
        int oldsize = size();

        T *oldarr = arr;
        arr = new T[newsize];
        int oldms = max_size;
        max_size = newsize;

        // newsize > oldsize
        if (newsize > oldsize) {
            if (_rear > _front) {
                int temp = 0;
                for (int i = _front; i <= _rear; i++) {
                    arr[temp++] = oldarr[i];
                }
                cout << temp << "    " << newsize << endl;

                while (temp < newsize) {
                    arr[temp] = data;
                    temp++;
                }

                _front = 0;
                _rear = newsize - 1;
            } else {
                for (int i = 0; i < newsize; i++) {
                    arr[i] = data;
                }
                int k = 0;
                int i = _front;
                int count = 0;

                for (; i <= oldms - 1; i++) {
                    if (count == oldsize)
                        break;
                    arr[k++] = oldarr[i];
                    count++;
                }
                _front = 0;
                if (count == oldsize)
                    _rear = newsize - 1;
                else {
                    i = 0;
                    for (; i <= _rear; i++) {
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
        else {
            if (_rear > _front) {
                int k = 0;
                for (int i = _front; i < newsize; i++) {
                    arr[k++] = oldarr[i];
                }
                _front = 0;
                _rear = newsize - 1;
            } else {
                int k = 0;
                int i = _front;
                int count = 0;

                for (; i <= oldms - 1; i++) {
                    if (count == newsize)
                        break;
                    arr[k++] = oldarr[i];
                    count++;
                }
                _front = 0;
                if (count == newsize)
                    _rear = k - 1;
                else {
                    i = 0;
                    for (; i <= _rear; i++) {
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

    // Delete the old array
    void clear() {
        T *oldarr = arr;
        _front = _rear = -1;
        max_size = 2;
        delete[] oldarr;
        arr = new T[max_size];
    }
};

std::string T;

std::vector<int> counting_sort_build_suffix() {
    int n = T.size();
    int alpha = ALPHABET;
    std::vector<int> p(n), r(n), count_arr(std::max(alpha, n), 0);

    for (size_t i = 0; i < n; i++) {
        count_arr[T[i]]++;
    }

    for (size_t i = 1; i < count_arr.size(); i++) {
        count_arr[i] += count_arr[i - 1];
    }

    for (size_t i = 0; i < n; i++) {
        count_arr[T[i]]--;
        p[count_arr[T[i]]] = i;
    }

    // Rank
    r[p[0]] = 0;
    int rank = 1;
    for (size_t i = 1; i < n; i++) {
        if (T[p[i]] != T[p[i - 1]])
            rank++;
        r[p[i]] = rank - 1;
    }

    std::vector<int> tempp(n), tempc(n);

    for (int h = 0; (1 << h) < n; ++h) {
        for (int i = 0; i < n; i++) {
            tempp[i] = p[i] - (1 << h);
            if (tempp[i] < 0)
                tempp[i] += n;
        }
        fill(count_arr.begin(), count_arr.begin() + rank, 0);
        for (int i = 0; i < n; i++)
            count_arr[r[tempp[i]]]++;
        for (int i = 1; i < rank; i++)
            count_arr[i] += count_arr[i - 1];
        for (int i = n - 1; i >= 0; i--)
            p[--count_arr[r[tempp[i]]]] = tempp[i];
        tempc[p[0]] = 0;
        rank = 1;
        for (int i = 1; i < n; i++) {
            Pair cur(r[p[i]], r[(p[i] + (1 << h)) % n]);
            Pair prev(r[p[i - 1]], r[(p[i - 1] + (1 << h)) % n]);
            if (cur.first != prev.first or cur.second != prev.second)
                ++rank;
            tempc[p[i]] = rank - 1;
        }
        r.swap(tempc);
    }
    return p;
}

std::vector<int> compute_lcp(std::string s, std::vector<int> sa, std::vector<int> rankarr) {
    int n = s.size(), k = 0;
    std::vector<int> lcp(n, 0);

    for (int i = 0; i < n; i++) rankarr[sa[i]] = i;

    for (int i = 0; i < n; i++, k ? k-- : 0) {
        if (rankarr[i] == n - 1) {
            k = 0;
            continue;
        }
        int j = sa[rankarr[i] + 1];
        while (i + k < n && j + k < n && s[i + k] == s[j + k]) k++;
        lcp[rankarr[i]] = k;
    }
    return lcp;
}

int maxInKWindow(std::vector<int> &lcp, int k) {
    int res = -1e8 + 7;
    Deque<int> dq;
    dq.deque();
    int tempk = 0;

    while (tempk < k) {
        if (!dq.empty()) {
            while (!dq.empty() and lcp[tempk] < lcp[dq.back()]) {
                dq.pop_back();
            }
        }

        dq.push_back(tempk);
        tempk++;
    }

    res = std::max(res, lcp[dq.front()]);
    tempk = k;

    while (tempk < lcp.size()) {
        while (!dq.empty() and dq.front() < (tempk - k + 1)) {
            dq.pop_front();
        }
        if (!dq.empty()) {
            while (!dq.empty() and lcp[tempk] < lcp[dq.back()]) {
                dq.pop_back();
            }
        }
        dq.push_back(tempk);

        res = std::max(res, lcp[dq.front()]);
        tempk++;
    }

    return res;
}

int main() {
    int n, k;
    cin >> k >> n;
    cin >> T;
    T.push_back('$');
    n = T.size();

    if (k == 1) {
        cout << n - 1 << "\n";
        return 0;
    }

    std::vector<int> sa = counting_sort_build_suffix();

    std::vector<int> rankarr(n, 0);
    for (int i = 0; i < n; i++) {
        rankarr[sa[i]] = i;
    }

    std::vector<int> lcparr = compute_lcp(T, sa, rankarr);

    int ans = 0;

    ans = maxInKWindow(lcparr, k - 1);

    if (ans == 0) {
        ans = -1;
    }

    cout << ans << "\n";

    return 0;
}