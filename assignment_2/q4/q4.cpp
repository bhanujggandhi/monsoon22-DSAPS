#include <iostream>

using namespace std;

struct Data {
    int row;
    int col;
    int value = 0;

    // Data(int _row, int _col, int _val) {
    //     row = _row;
    //     col = _col;
    //     value = _val;
    // }
};

struct SparseMatrix {
    int rows;
    int cols;
    Data *matrix;
    int ind;

    SparseMatrix(int r, int c) {
        rows = r;
        cols = c;
        matrix = new Data[rows * cols];
        ind = 0;
    }

    void setMatrix(Data *mat) { matrix = mat; }
};

void insert_elements(SparseMatrix &m, int row, int col, int val) {
    if (row < 0 or row >= m.rows or col < 0 or col >= m.cols) return;

    m.matrix[m.ind++] = {row, col, val};
}

void printmatrix(SparseMatrix &m) {
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

void add(SparseMatrix m1, SparseMatrix m2) {
    // For additions rows and cols should be same
    if (m1.rows != m2.rows or m1.cols != m2.cols)
        return;
    else {
        SparseMatrix result(m1.rows, m1.cols);
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

SparseMatrix transpose(SparseMatrix m) {
    int total[m.cols] = {0};
    int index[m.cols + 1];

    for (int i = 0; i < m.ind; i++) {
        total[m.matrix[i].col]++;
    }

    index[0] = 0;
    for (int i = 1; i <= m.cols; i++) {
        index[i] = index[i - 1] + total[i - 1];
    }

    SparseMatrix result(m.rows, m.cols);
    result.ind = m.ind;
    Data *arr = new Data[m.ind];

    for (int i = 0; i < m.ind; i++) {
        int ind = index[m.matrix[i].col];
        arr[ind].col = m.matrix[i].row;
        arr[ind].row = m.matrix[i].col;
        arr[ind].value = m.matrix[i].value;
        index[m.matrix[i].col]++;
    }

    result.setMatrix(arr);

    // printmatrix(result);

    return result;
}

void multiply(SparseMatrix m1, SparseMatrix m2) {
    // Transpose m2
    //

    if (m1.cols != m2.cols) return;

    SparseMatrix result(m1.rows, m2.rows);

    for (int i = 0; i < m1.ind; i++) {
        cout << m1.matrix[i].row << "\t" << m1.matrix[i].col << "\t"
             << m1.matrix[i].value << endl;
    }

    cout << "----------------------------------" << endl;

    for (int i = 0; i < m2.ind; i++) {
        cout << m2.matrix[i].row << "\t" << m2.matrix[i].col << "\t"
             << m2.matrix[i].value << endl;
    }
    for (int i = 0; i < m1.ind; i++) {
        for (int j = 0; j < m2.ind; j++) {
            if (m1.matrix[i].col == m2.matrix[j].col) {
            }
        }
    }
}

int main() {
    // m->rows
    // n->cols
    int m1, n1;
    cin >> m1 >> n1;

    SparseMatrix sm1(m1, n1);
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

    SparseMatrix sm2(m2, n2);
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

    multiply(sm1, transpose(sm2));

    return 0;
}