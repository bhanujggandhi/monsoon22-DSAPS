#include <algorithm>
#include <iostream>

using namespace std;

// -------------------Data Types-----------------------
// For Primitive Data Types only (Except String chars)
template <class T>
struct ArrData {
    int row;
    int col;
    T value = 0;
};

template <class T>
struct LLData {
    int row;
    int col;
    T value;
    LLData<T> *next;
    // LLData<T> *prev;

    LLData(int r, int c, T v) {
        row = r;
        col = c;
        value = v;
        next = NULL;
        // prev = NULL;
    }
};

// -------------------Array Based Implementation-----------------------
template <class T>
class ArrSparseMatrix {
   public:
    int rows;
    int cols;
    ArrData<T> *matrix;
    int ind;

    ArrSparseMatrix(int r, int c) {
        rows = r;
        cols = c;
        matrix = new ArrData<T>[rows * cols];
        ind = 0;
    }

    // -------------------Utility-----------------------
    void setMatrix(ArrData<T> *mat) { matrix = mat; }

    static bool cmp(ArrData<T> a, ArrData<T> b) {
        if (a.row == b.row) return a.col < b.col;

        return a.row < b.row;
    }

    void arrInsertElements(int row, int col, T val) {
        if (row < 0 or row >= rows or col < 0 or col >= cols) return;

        matrix[ind++] = {row, col, val};
    }

    void arrPrintMatrix(ArrSparseMatrix<T> &m) {
        int k = 0;
        for (int i = 0; i < m.rows; i++) {
            for (int j = 0; j < m.cols; j++) {
                if (k < m.ind and i == m.matrix[k].row and j == m.matrix[k].col)
                    cout << m.matrix[k++].value << "\t";
                else
                    cout << 0 << "\t";
            }
            cout << "\n";
        }
    }

    void operator+(ArrSparseMatrix<T> m2) {
        // For additions rows and cols should be same
        if (rows != m2.rows or cols != m2.cols)
            return;
        else {
            ArrSparseMatrix<T> result(rows, cols);
            int i = 0, j = 0, k = 0;

            while (i < ind and j < m2.ind) {
                if (matrix[i].row == m2.matrix[j].row) {
                    if (matrix[i].col == m2.matrix[j].col) {
                        if (matrix[i].value + m2.matrix[j].value != 0) {
                            result.arrInsertElements(
                                matrix[i].row, matrix[i].col,
                                matrix[i].value + m2.matrix[j].value);
                        }
                        i++;
                        j++;
                    } else if (matrix[i].col < m2.matrix[j].col) {
                        result.arrInsertElements(matrix[i].row, matrix[i].col,
                                                 matrix[i].value);
                        i++;
                    } else {
                        result.arrInsertElements(m2.matrix[j].row,
                                                 m2.matrix[j].col,
                                                 m2.matrix[j].value);
                        j++;
                    }
                } else if (matrix[i].row < m2.matrix[i].row) {
                    result.arrInsertElements(matrix[i].row, matrix[i].col,
                                             matrix[i].value);
                    i++;
                } else {
                    result.arrInsertElements(m2.matrix[j].row, m2.matrix[j].col,
                                             m2.matrix[j].value);
                    j++;
                }
            }

            while (i < ind) {
                result.arrInsertElements(matrix[i].row, matrix[i].col,
                                         matrix[i].value);
                i++;
            }

            while (j < m2.ind) {
                result.arrInsertElements(m2.matrix[j].row, m2.matrix[j].col,
                                         m2.matrix[j].value);
                j++;
            }

            arrPrintMatrix(result);
        }
    }

    ArrSparseMatrix<T> arrTranspose(ArrSparseMatrix<T> m) {
        int total[m.cols] = {0};
        int index[m.cols + 1];

        for (int i = 0; i < m.ind; i++) {
            total[m.matrix[i].col]++;
        }

        index[0] = 0;
        for (int i = 1; i <= m.cols; i++) {
            index[i] = index[i - 1] + total[i - 1];
        }

        ArrSparseMatrix<T> result(m.rows, m.cols);
        result.ind = m.ind;
        ArrData<T> *arr = new ArrData<T>[m.ind];

        for (int i = 0; i < m.ind; i++) {
            int ind = index[m.matrix[i].col];
            arr[ind].col = m.matrix[i].row;
            arr[ind].row = m.matrix[i].col;
            arr[ind].value = m.matrix[i].value;
            index[m.matrix[i].col]++;
        }

        result.setMatrix(arr);

        return result;
    }

    void operator*(ArrSparseMatrix<T> m2) {
        // Multiply col1 x row2
        // Sort
        // Remove duplicates

        if (cols != m2.rows) return;

        ArrSparseMatrix<T> result(rows, m2.cols);

        for (int i = 0; i < ind; i++) {
            for (int j = 0; j < m2.ind; j++) {
                if (matrix[i].col == m2.matrix[j].row) {
                    result.arrInsertElements(
                        matrix[i].row, m2.matrix[j].col,
                        matrix[i].value * m2.matrix[j].value);
                }
            }
        }

        sort(result.matrix, result.matrix + result.ind, result.cmp);

        ArrSparseMatrix<T> ans(result.rows, result.cols);

        for (int i = 0; i < result.ind; i++) {
            if (i + 1 < result.ind) {
                if (result.matrix[i].row == result.matrix[i + 1].row and
                    result.matrix[i].col == result.matrix[i + 1].col) {
                    ArrData<T> temp = {result.matrix[i].row,
                                       result.matrix[i].col,
                                       result.matrix[i].value};

                    while (i + 1 < result.ind and
                           result.matrix[i].row == result.matrix[i + 1].row and
                           result.matrix[i].col == result.matrix[i + 1].col) {
                        temp.value += result.matrix[i + 1].value;
                        i++;
                    }
                    ans.arrInsertElements(temp.row, temp.col, temp.value);
                } else {
                    ans.arrInsertElements(result.matrix[i].row,
                                          result.matrix[i].col,
                                          result.matrix[i].value);
                }
            }
        }

        arrPrintMatrix(ans);
    }
};

// ---------------Linked List Based Implementation-------------------

template <class T>
class LLSparseMatrix {
   public:
    int rows;
    int cols;
    LLData<T> *head;
    LLData<T> *tail;
    int size;

    LLSparseMatrix(int r, int c) {
        rows = r;
        cols = c;
        head = NULL;
        tail = NULL;
        size = 0;
    }

    // -------------------Utility-----------------------
    void setHead(LLData<T> *node) { head = node; }

    LLData<T> *merge(LLData<T> *left, LLData<T> *right) {
        if (left == NULL) return right;

        if (right == NULL) return left;

        LLData<T> *head = new LLData<T>(-1, -1, -1);
        LLData<T> *tail = head;

        while (left != NULL and right != NULL) {
            if (left->row < right->row) {
                tail->next = left;
                tail = tail->next;
                left = left->next;
                tail->next = NULL;
            } else if (left->row == right->row) {
                if (left->col < right->col) {
                    tail->next = left;
                    tail = tail->next;
                    left = left->next;
                    tail->next = NULL;
                } else {
                    tail->next = right;
                    tail = tail->next;
                    right = right->next;
                    tail->next = NULL;
                }
            } else {
                tail->next = right;
                tail = tail->next;
                right = right->next;
                tail->next = NULL;
            }
        }

        while (left != NULL) {
            tail->next = left;
            tail = tail->next;
            left = left->next;
            tail->next = NULL;
        }

        while (right != NULL) {
            tail->next = right;
            tail = tail->next;
            right = right->next;
            tail->next = NULL;
        }

        tail->next = NULL;
        return head->next;
    }

    LLData<T> *mergeSort(LLData<T> *head) {
        if (head == NULL or head->next == NULL) return head;

        LLData<T> *prev = NULL;
        LLData<T> *mid = head;
        LLData<T> *a = head;

        while (a != NULL and a->next != NULL) {
            prev = mid;
            a = a->next->next;
            mid = mid->next;
        }

        if (prev != NULL) prev->next = NULL;

        LLData<T> *right = NULL, *left = NULL;

        left = mergeSort(head);
        right = mergeSort(mid);

        return merge(left, right);
    }

    void llInsertElements(int row, int col, T val) {
        if (size == 0) {
            head = tail = new LLData<T>(row, col, val);
            size++;
        } else {
            LLData<T> *nnode = new LLData<T>(row, col, val);
            tail->next = nnode;
            tail = tail->next;
            size++;
        }
    }

    void llPrintMatrix(LLSparseMatrix<T> m) {
        LLData<T> *temp = m.head;
        for (int i = 0; i < m.rows; i++) {
            for (int j = 0; j < m.cols; j++) {
                if (temp != NULL and i == temp->row and j == temp->col) {
                    cout << temp->value << "\t";
                    temp = temp->next;
                } else
                    cout << 0 << "\t";
            }
            cout << "\n";
        }
    }

    void operator+(LLSparseMatrix<T> m2) {
        if (rows != m2.rows or cols != m2.cols)
            return;
        else {
            LLSparseMatrix<T> result(rows, cols);
            LLData<T> *temp1 = head, *temp2 = head, *start = NULL;

            while (temp1 != NULL and temp2 != NULL) {
                if (temp1->row == temp2->row) {
                    if (temp1->col == temp2->col) {
                        if (temp1->value + temp2->value != 0) {
                            result.llInsertElements(
                                temp1->row, temp1->col,
                                temp1->value + temp2->value);
                        }
                        temp1 = temp1->next;
                        temp2 = temp2->next;
                    } else if (temp1->col < temp2->col) {
                        result.llInsertElements(temp1->row, temp1->col,
                                                temp1->value);
                        temp1 = temp1->next;
                    } else {
                        result.llInsertElements(temp2->row, temp2->col,
                                                temp2->value);
                        temp2 = temp2->next;
                    }
                } else if (temp1->row < temp2->row) {
                    result.llInsertElements(temp1->row, temp1->col,
                                            temp1->value);
                    temp1 = temp1->next;
                } else {
                    result.llInsertElements(temp2->row, temp2->col,
                                            temp2->value);
                    temp2 = temp2->next;
                }
            }

            while (temp1 != NULL) {
                result.llInsertElements(temp1->row, temp1->col, temp1->value);
                temp1 = temp1->next;
            }

            while (temp2 != NULL) {
                result.llInsertElements(temp2->row, temp2->col, temp2->value);
                temp2 = temp2->next;
            }

            llPrintMatrix(result);
        }
    }

    LLSparseMatrix<T> llTranspose(LLSparseMatrix<T> m) {
        LLData<T> *temp = m.head;
        while (temp) {
            int t = temp->row;
            temp->row = temp->col;
            temp->col = t;
            temp = temp->next;
        }

        m.head = mergeSort(m.head);

        return m;
    }

    void operator*(LLSparseMatrix<T> m2) {
        if (cols != m2.rows) return;

        LLSparseMatrix<T> result(rows, m2.cols);

        for (LLData<T> *temp1 = head; temp1 != NULL; temp1 = temp1->next) {
            for (LLData<T> *temp2 = m2.head; temp2 != NULL;
                 temp2 = temp2->next) {
                if (temp1->col == temp2->row) {
                    result.llInsertElements(temp1->row, temp2->col,
                                            temp1->value * temp2->value);
                }
            }
        }

        result.head = mergeSort(result.head);

        LLSparseMatrix<T> ans(result.rows, result.cols);

        for (LLData<T> *temp = result.head; temp != NULL; temp = temp->next) {
            if (temp->next != NULL) {
                if (temp->row == temp->next->row and
                    temp->col == temp->next->col) {
                    LLData<T> *newnode =
                        new LLData<T>(temp->row, temp->col, temp->value);
                    while (temp->next != NULL and
                           temp->row == temp->next->row and
                           temp->col == temp->next->col) {
                        newnode->value += temp->next->value;
                        temp = temp->next;
                    }
                    ans.llInsertElements(newnode->row, newnode->col,
                                         newnode->value);
                } else {
                    ans.llInsertElements(temp->row, temp->col, temp->value);
                }
            }
        }

        llPrintMatrix(ans);
    }
};

// -------------------- Driver Code ----------------------------
int main() {
    int mode;
    std::cin >> mode;

    if (mode == 1) {
        int operation;
        std::cin >> operation;
        if (operation == 1) {
            int n1, m1;
            std::cin >> n1 >> m1;
            ArrSparseMatrix<float> sm1(n1, m1);

            for (int i = 0; i < n1; i++) {
                for (int j = 0; j < m1; j++) {
                    float k;
                    std::cin >> k;
                    if (k != 0) sm1.arrInsertElements(i, j, k);
                }
            }

            int n2, m2;
            std::cin >> n2 >> m2;
            ArrSparseMatrix<float> sm2(n2, m2);
            for (int i = 0; i < n2; i++) {
                for (int j = 0; j < m2; j++) {
                    float k;
                    std::cin >> k;
                    if (k != 0) sm2.arrInsertElements(i, j, k);
                }
            }
            sm1 + sm2;
        } else if (operation == 2) {
            int n1, m1;
            std::cin >> n1 >> m1;
            ArrSparseMatrix<float> sm1(n1, m1);

            for (int i = 0; i < n1; i++) {
                for (int j = 0; j < m1; j++) {
                    float k;
                    std::cin >> k;
                    if (k != 0) sm1.arrInsertElements(i, j, k);
                }
            }
            sm1 = sm1.arrTranspose(sm1);
            sm1.arrPrintMatrix(sm1);
        } else if (operation == 3) {
            int n1, m1;
            std::cin >> n1 >> m1;
            ArrSparseMatrix<float> sm1(n1, m1);

            for (int i = 0; i < n1; i++) {
                for (int j = 0; j < m1; j++) {
                    float k;
                    std::cin >> k;
                    if (k != 0) sm1.arrInsertElements(i, j, k);
                }
            }

            int n2, m2;
            std::cin >> n2 >> m2;
            ArrSparseMatrix<float> sm2(n2, m2);
            for (int i = 0; i < n2; i++) {
                for (int j = 0; j < m2; j++) {
                    float k;
                    std::cin >> k;
                    if (k != 0) sm2.arrInsertElements(i, j, k);
                }
            }
            sm1 *sm2;
        }
    } else if (mode == 2) {
        int operation;
        std::cin >> operation;
        if (operation == 1) {
            int n1, m1;
            std::cin >> n1 >> m1;
            LLSparseMatrix<int> sm1(n1, m1);

            for (int i = 0; i < n1; i++) {
                for (int j = 0; j < m1; j++) {
                    int k;
                    std::cin >> k;
                    if (k != 0) sm1.llInsertElements(i, j, k);
                }
            }

            int n2, m2;
            std::cin >> n2 >> m2;
            LLSparseMatrix<int> sm2(n2, m2);
            for (int i = 0; i < n2; i++) {
                for (int j = 0; j < m2; j++) {
                    int k;
                    std::cin >> k;
                    if (k != 0) sm2.llInsertElements(i, j, k);
                }
            }
            sm1 + sm2;
        } else if (operation == 2) {
            int n1, m1;
            std::cin >> n1 >> m1;
            LLSparseMatrix<int> sm1(n1, m1);

            for (int i = 0; i < n1; i++) {
                for (int j = 0; j < m1; j++) {
                    int k;
                    std::cin >> k;
                    if (k != 0) sm1.llInsertElements(i, j, k);
                }
            }

            sm1.llPrintMatrix(sm1.llTranspose(sm1));
        } else if (operation == 3) {
            int n1, m1;
            std::cin >> n1 >> m1;
            LLSparseMatrix<int> sm1(n1, m1);

            for (int i = 0; i < n1; i++) {
                for (int j = 0; j < m1; j++) {
                    int k;
                    std::cin >> k;
                    if (k != 0) sm1.llInsertElements(i, j, k);
                }
            }

            int n2, m2;
            std::cin >> n2 >> m2;
            LLSparseMatrix<int> sm2(n2, m2);
            for (int i = 0; i < n2; i++) {
                for (int j = 0; j < m2; j++) {
                    int k;
                    std::cin >> k;
                    if (k != 0) sm2.llInsertElements(i, j, k);
                }
            }
            sm1 *sm2;
        }
    }
    return 0;
}

/*

4 4
0 10.2 4.1 2
0 0 0 0
0 0 3.4 0
4.9 2.1 0 0
4 4
0 0 0 2.4
0 0 2.1 7.9
8.1 0 9.1 0
0 3.4 6.2 0

*/