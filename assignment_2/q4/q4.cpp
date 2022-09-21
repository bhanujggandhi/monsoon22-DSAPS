#include <algorithm>
#include <iostream>

using namespace std;

// -------------------Data Types-----------------------
struct ArrData {
    int row;
    int col;
    int value = 0;
};

struct ArrSparseMatrix {
    int rows;
    int cols;
    ArrData *matrix;
    int ind;

    ArrSparseMatrix(int r, int c) {
        rows = r;
        cols = c;
        matrix = new ArrData[rows * cols];
        ind = 0;
    }

    void setMatrix(ArrData *mat) { matrix = mat; }
};

struct LLData {
    int row;
    int col;
    int value;
    LLData *next;
    // LLData *prev;

    LLData(int r, int c, int v) {
        row = r;
        col = c;
        value = v;
        next = NULL;
        // prev = NULL;
    }
};

struct LLSparseMatrix {
    int rows;
    int cols;
    LLData *head;
    LLData *tail;
    int size;

    LLSparseMatrix(int r, int c) {
        rows = r;
        cols = c;
        head = NULL;
        size = 0;
    }

    void setHead(LLData *node) { head = node; }
};

// -------------------Utility-----------------------

bool cmp(ArrData a, ArrData b) {
    if (a.row == b.row) return a.col < b.col;

    return a.row < b.row;
}

LLData *merge(LLData *left, LLData *right) {
    if (left == NULL) return right;

    if (right == NULL) return left;

    LLData *head = new LLData(-1, -1, -1);
    LLData *tail = head;

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

LLData *mergeSort(LLData *head) {
    if (head == NULL or head->next == NULL) return head;

    LLData *prev = NULL;
    LLData *mid = head;
    LLData *a = head;

    while (a != NULL and a->next != NULL) {
        prev = mid;
        a = a->next->next;
        mid = mid->next;
    }

    if (prev != NULL) prev->next = NULL;

    LLData *right = NULL, *left = NULL;

    left = mergeSort(head);
    right = mergeSort(mid);

    return merge(left, right);
}

void printlist(LLData *node) {
    while (node) {
        std::cout << node->row << "\t" << node->col << "\t" << node->value
                  << std::endl;
        node = node->next;
    }
}

// -------------------Array Based Implementation-----------------------

void arrInsertElements(ArrSparseMatrix &m, int row, int col, int val) {
    if (row < 0 or row >= m.rows or col < 0 or col >= m.cols) return;

    m.matrix[m.ind++] = {row, col, val};
}

void arrPrintMatrix(ArrSparseMatrix &m) {
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

void arrAdd(ArrSparseMatrix m1, ArrSparseMatrix m2) {
    // For additions rows and cols should be same
    if (m1.rows != m2.rows or m1.cols != m2.cols)
        return;
    else {
        ArrSparseMatrix result(m1.rows, m1.cols);
        int i = 0, j = 0, k = 0;

        while (i < m1.ind and j < m2.ind) {
            if (m1.matrix[i].row == m2.matrix[j].row) {
                if (m1.matrix[i].col == m2.matrix[j].col) {
                    if (m1.matrix[i].value + m2.matrix[j].value != 0) {
                        arrInsertElements(
                            result, m1.matrix[i].row, m1.matrix[i].col,
                            m1.matrix[i].value + m2.matrix[j].value);
                    }
                    i++;
                    j++;
                } else if (m1.matrix[i].col < m2.matrix[j].col) {
                    arrInsertElements(result, m1.matrix[i].row,
                                      m1.matrix[i].col, m1.matrix[i].value);
                    i++;
                } else {
                    arrInsertElements(result, m2.matrix[j].row,
                                      m2.matrix[j].col, m2.matrix[j].value);
                    j++;
                }
            } else if (m1.matrix[i].row < m2.matrix[i].row) {
                arrInsertElements(result, m1.matrix[i].row, m1.matrix[i].col,
                                  m1.matrix[i].value);
                i++;
            } else {
                arrInsertElements(result, m2.matrix[j].row, m2.matrix[j].col,
                                  m2.matrix[j].value);
                j++;
            }
        }

        while (i < m1.ind) {
            arrInsertElements(result, m1.matrix[i].row, m1.matrix[i].col,
                              m1.matrix[i].value);
            i++;
        }

        while (j < m2.ind) {
            arrInsertElements(result, m2.matrix[j].row, m2.matrix[j].col,
                              m2.matrix[j].value);
            j++;
        }

        arrPrintMatrix(result);
    }
}

ArrSparseMatrix arrTranspose(ArrSparseMatrix m) {
    int total[m.cols] = {0};
    int index[m.cols + 1];

    for (int i = 0; i < m.ind; i++) {
        total[m.matrix[i].col]++;
    }

    index[0] = 0;
    for (int i = 1; i <= m.cols; i++) {
        index[i] = index[i - 1] + total[i - 1];
    }

    ArrSparseMatrix result(m.rows, m.cols);
    result.ind = m.ind;
    ArrData *arr = new ArrData[m.ind];

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

void arrMultiply(ArrSparseMatrix m1, ArrSparseMatrix m2) {
    // Multiply col1 x row2
    // Sort
    // Remove duplicates

    if (m1.cols != m2.rows) return;

    ArrSparseMatrix result(m1.rows, m2.cols);

    for (int i = 0; i < m1.ind; i++) {
        for (int j = 0; j < m2.ind; j++) {
            if (m1.matrix[i].col == m2.matrix[j].row) {
                arrInsertElements(result, m1.matrix[i].row, m2.matrix[j].col,
                                  m1.matrix[i].value * m2.matrix[j].value);
            }
        }
    }

    sort(result.matrix, result.matrix + result.ind, cmp);

    ArrSparseMatrix ans(result.rows, result.cols);

    for (int i = 0; i < result.ind; i++) {
        if (i + 1 < result.ind) {
            if (result.matrix[i].row == result.matrix[i + 1].row and
                result.matrix[i].col == result.matrix[i + 1].col) {
                ArrData temp = {result.matrix[i].row, result.matrix[i].col,
                                result.matrix[i].value};

                while (i + 1 < result.ind and
                       result.matrix[i].row == result.matrix[i + 1].row and
                       result.matrix[i].col == result.matrix[i + 1].col) {
                    temp.value += result.matrix[i + 1].value;
                    i++;
                }
                arrInsertElements(ans, temp.row, temp.col, temp.value);
            } else {
                arrInsertElements(ans, result.matrix[i].row,
                                  result.matrix[i].col, result.matrix[i].value);
            }
        }
    }

    arrPrintMatrix(ans);
}

// -------------------Linked List Based Implementation-----------------------

void llInsertElements(LLSparseMatrix &m, int row, int col, int val) {
    if (m.size == 0) {
        m.head = m.tail = new LLData(row, col, val);
        m.size++;
    } else {
        LLData *nnode = new LLData(row, col, val);
        m.tail->next = nnode;
        m.tail = m.tail->next;
        m.size++;
    }
}

void llPrintMatrix(LLSparseMatrix m) {
    LLData *temp = m.head;
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

void llAdd(LLSparseMatrix m1, LLSparseMatrix m2) {
    if (m1.rows != m2.rows or m1.cols != m2.cols)
        return;
    else {
        LLSparseMatrix result(m1.rows, m1.cols);
        LLData *temp1 = m1.head, *temp2 = m2.head, *start = NULL;

        while (temp1 != NULL and temp2 != NULL) {
            if (temp1->row == temp2->row) {
                if (temp1->col == temp2->col) {
                    if (temp1->value + temp2->value != 0) {
                        llInsertElements(result, temp1->row, temp1->col,
                                         temp1->value + temp2->value);
                    }
                    temp1 = temp1->next;
                    temp2 = temp2->next;
                } else if (temp1->col < temp2->col) {
                    llInsertElements(result, temp1->row, temp1->col,
                                     temp1->value);
                    temp1 = temp1->next;
                } else {
                    llInsertElements(result, temp2->row, temp2->col,
                                     temp2->value);
                    temp2 = temp2->next;
                }
            } else if (temp1->row < temp2->row) {
                llInsertElements(result, temp1->row, temp1->col, temp1->value);
                temp1 = temp1->next;
            } else {
                llInsertElements(result, temp2->row, temp2->col, temp2->value);
                temp2 = temp2->next;
            }
        }

        while (temp1 != NULL) {
            llInsertElements(result, temp1->row, temp1->col, temp1->value);
            temp1 = temp1->next;
        }

        while (temp2 != NULL) {
            llInsertElements(result, temp2->row, temp2->col, temp2->value);
            temp2 = temp2->next;
        }

        llPrintMatrix(result);
    }
}

LLSparseMatrix llTranspose(LLSparseMatrix m) {
    LLData *temp = m.head;
    while (temp) {
        int t = temp->row;
        temp->row = temp->col;
        temp->col = t;
        temp = temp->next;
    }

    m.head = mergeSort(m.head);

    return m;
}

void llMultiply(LLSparseMatrix m1, LLSparseMatrix m2) {
    if (m1.cols != m2.rows) return;

    LLSparseMatrix result(m1.rows, m2.cols);

    for (LLData *temp1 = m1.head; temp1 != NULL; temp1 = temp1->next) {
        for (LLData *temp2 = m2.head; temp2 != NULL; temp2 = temp2->next) {
            if (temp1->col == temp2->row) {
                llInsertElements(result, temp1->row, temp2->col,
                                 temp1->value * temp2->value);
            }
        }
    }

    result.head = mergeSort(result.head);

    LLSparseMatrix ans(result.rows, result.cols);

    for (LLData *temp = result.head; temp != NULL; temp = temp->next) {
        if (temp->next != NULL) {
            if (temp->row == temp->next->row and temp->col == temp->next->col) {
                LLData *newnode = new LLData(temp->row, temp->col, temp->value);
                while (temp->next != NULL and temp->row == temp->next->row and
                       temp->col == temp->next->col) {
                    newnode->value += temp->next->value;
                    temp = temp->next;
                }
                llInsertElements(ans, newnode->row, newnode->col,
                                 newnode->value);
            } else {
                llInsertElements(ans, temp->row, temp->col, temp->value);
            }
        }
    }

    llPrintMatrix(ans);
}

int main() {
    int mode;
    std::cin >> mode;

    if (mode == 1) {
        int operation;
        std::cin >> operation;
        if (operation == 1) {
            int n1, m1;
            std::cin >> n1 >> m1;
            ArrSparseMatrix sm1(n1, m1);

            for (int i = 0; i < n1; i++) {
                for (int j = 0; j < m1; j++) {
                    int k;
                    std::cin >> k;
                    if (k != 0) arrInsertElements(sm1, i, j, k);
                }
            }

            int n2, m2;
            std::cin >> n2 >> m2;
            ArrSparseMatrix sm2(n2, m2);
            for (int i = 0; i < n2; i++) {
                for (int j = 0; j < m2; j++) {
                    int k;
                    std::cin >> k;
                    if (k != 0) arrInsertElements(sm2, i, j, k);
                }
            }
            arrAdd(sm1, sm2);
        } else if (operation == 2) {
            int n1, m1;
            std::cin >> n1 >> m1;
            ArrSparseMatrix sm1(n1, m1);

            for (int i = 0; i < n1; i++) {
                for (int j = 0; j < m1; j++) {
                    int k;
                    std::cin >> k;
                    if (k != 0) arrInsertElements(sm1, i, j, k);
                }
            }
            sm1 = arrTranspose(sm1);
            arrPrintMatrix(sm1);
        } else if (operation == 3) {
            int n1, m1;
            std::cin >> n1 >> m1;
            ArrSparseMatrix sm1(n1, m1);

            for (int i = 0; i < n1; i++) {
                for (int j = 0; j < m1; j++) {
                    int k;
                    std::cin >> k;
                    if (k != 0) arrInsertElements(sm1, i, j, k);
                }
            }

            int n2, m2;
            std::cin >> n2 >> m2;
            ArrSparseMatrix sm2(n2, m2);
            for (int i = 0; i < n2; i++) {
                for (int j = 0; j < m2; j++) {
                    int k;
                    std::cin >> k;
                    if (k != 0) arrInsertElements(sm2, i, j, k);
                }
            }
            arrMultiply(sm1, sm2);
        }
    } else if (mode == 2) {
        int operation;
        std::cin >> operation;
        if (operation == 1) {
            int n1, m1;
            std::cin >> n1 >> m1;
            LLSparseMatrix sm1(n1, m1);

            for (int i = 0; i < n1; i++) {
                for (int j = 0; j < m1; j++) {
                    int k;
                    std::cin >> k;
                    if (k != 0) llInsertElements(sm1, i, j, k);
                }
            }

            int n2, m2;
            std::cin >> n2 >> m2;
            LLSparseMatrix sm2(n2, m2);
            for (int i = 0; i < n2; i++) {
                for (int j = 0; j < m2; j++) {
                    int k;
                    std::cin >> k;
                    if (k != 0) llInsertElements(sm2, i, j, k);
                }
            }
            llAdd(sm1, sm2);
        } else if (operation == 2) {
            int n1, m1;
            std::cin >> n1 >> m1;
            LLSparseMatrix sm1(n1, m1);

            for (int i = 0; i < n1; i++) {
                for (int j = 0; j < m1; j++) {
                    int k;
                    std::cin >> k;
                    if (k != 0) llInsertElements(sm1, i, j, k);
                }
            }

            llPrintMatrix(llTranspose(sm1));
        } else if (operation == 3) {
            int n1, m1;
            std::cin >> n1 >> m1;
            LLSparseMatrix sm1(n1, m1);

            for (int i = 0; i < n1; i++) {
                for (int j = 0; j < m1; j++) {
                    int k;
                    std::cin >> k;
                    if (k != 0) llInsertElements(sm1, i, j, k);
                }
            }

            int n2, m2;
            std::cin >> n2 >> m2;
            LLSparseMatrix sm2(n2, m2);
            for (int i = 0; i < n2; i++) {
                for (int j = 0; j < m2; j++) {
                    int k;
                    std::cin >> k;
                    if (k != 0) llInsertElements(sm2, i, j, k);
                }
            }
            llMultiply(sm1, sm2);
        }
    }
    return 0;
}

/*

4 4
0 10 4 2
0 0 0 0
0 0 3 0
4 2 0 0
4 4
0 0 0 2
0 0 2 7
8 0 9 0
0 3 6 0

*/