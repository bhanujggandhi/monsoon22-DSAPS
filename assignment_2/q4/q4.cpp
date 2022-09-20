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
    LLData *prev;

    LLData(int r, int c, int v) {
        row = r;
        col = c;
        value = v;
        next = NULL;
        prev = NULL;
    }
};

struct LLSparseMatrix {
    int rows;
    int cols;
    LLData *head;
    int size;

    LLSparseMatrix(int r, int c) {
        rows = r;
        cols = c;
        head = NULL;
        size = 0;
    }

    void setHead(LLData *node) { head = node; }
};

// -------------------Array Based Implementation-----------------------

void insert_elements(ArrSparseMatrix &m, int row, int col, int val) {
    if (row < 0 or row >= m.rows or col < 0 or col >= m.cols) return;

    m.matrix[m.ind++] = {row, col, val};
}

void printmatrix(ArrSparseMatrix &m) {
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

void add(ArrSparseMatrix m1, ArrSparseMatrix m2) {
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
                        insert_elements(
                            result, m1.matrix[i].row, m1.matrix[i].col,
                            m1.matrix[i].value + m2.matrix[j].value);
                    }
                    i++;
                    j++;
                } else if (m1.matrix[i].col < m2.matrix[j].col) {
                    insert_elements(result, m1.matrix[i].row, m1.matrix[i].col,
                                    m1.matrix[i].value);
                    i++;
                } else {
                    insert_elements(result, m2.matrix[j].row, m2.matrix[j].col,
                                    m2.matrix[j].value);
                    j++;
                }
            } else if (m1.matrix[i].row < m2.matrix[i].row) {
                insert_elements(result, m1.matrix[i].row, m1.matrix[i].col,
                                m1.matrix[i].value);
                i++;
            } else {
                insert_elements(result, m2.matrix[j].row, m2.matrix[j].col,
                                m2.matrix[j].value);
                j++;
            }
        }

        while (i < m1.ind) {
            insert_elements(result, m1.matrix[i].row, m1.matrix[i].col,
                            m1.matrix[i].value);
            i++;
        }

        while (j < m2.ind) {
            insert_elements(result, m2.matrix[j].row, m2.matrix[j].col,
                            m2.matrix[j].value);
            j++;
        }

        printmatrix(result);
    }
}

ArrSparseMatrix transpose(ArrSparseMatrix m) {
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

bool cmp(ArrData a, ArrData b) {
    if (a.row == b.row) return a.col < b.col;

    return a.row < b.row;
}

void multiply(ArrSparseMatrix m1, ArrSparseMatrix m2) {
    // Multiply col1 x row2
    // Sort
    // Remove duplicates

    if (m1.cols != m2.cols) return;

    ArrSparseMatrix result(m1.rows, m2.rows);

    for (int i = 0; i < m1.ind; i++) {
        for (int j = 0; j < m2.ind; j++) {
            if (m1.matrix[i].col == m2.matrix[j].row) {
                insert_elements(result, m1.matrix[i].row, m2.matrix[j].col,
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
                insert_elements(ans, temp.row, temp.col, temp.value);
            } else {
                insert_elements(ans, result.matrix[i].row, result.matrix[i].col,
                                result.matrix[i].value);
            }
        }
    }

    printmatrix(ans);
}

// -------------------Linked List Based Implementation-----------------------

int main() {
    // m->rows
    // n->cols
    int m1, n1;
    cin >> m1 >> n1;

    ArrSparseMatrix sm1(m1, n1);
    for (int i = 0; i < m1; i++) {
        for (int j = 0; j < n1; j++) {
            int k;
            cin >> k;
            if (k != 0) {
                insert_elements(sm1, i, j, k);
            }
        }
    }

    int m2, n2;
    cin >> m2 >> n2;

    ArrSparseMatrix sm2(m2, n2);
    for (int i = 0; i < m2; i++) {
        for (int j = 0; j < n2; j++) {
            int k;
            cin >> k;
            if (k != 0) {
                insert_elements(sm2, i, j, k);
            }
        }
    }

    // add(sm1, sm2);
    // transpose(sm1);

    multiply(sm1, sm2);

    // 4 4
    // 0 10 0 12
    // 0 0 0 0
    // 0 0 5 0
    // 15 12 0 0
    // 4 4
    // 0 0 8 0
    // 0 0 0 23
    // 0 0 9 0
    // 20 25 0 0

    // 4 4
    // 0 10 4 2
    // 0 0 0 0
    // 0 0 3 0
    // 4 2 0 1
    // 4 4
    // 0 0 0 2
    // 0 0 2 7
    // 8 0 9 0
    // 0 3 6 6
    return 0;
}