#include <iostream>
using namespace std;
#define Type int
// Variables declarations...
//  |**************************| To generic change define to that type
//  |***********************|
Type value, Matrix1[100000][3], Matrix2[100000][3], Res_Mat[100000][3], Sum,
    k;  // Template to change...

int choice1, choice2;
template <typename T>
class Node {
   public:
    int row;
    int col;
    T value;
    Node* next;
    Node(int r, int c, T val) {
        row = r;
        col = c;
        value = val;
        next = NULL;
    }
};
template <typename T>
Node<Type>* create_node(int r, int c, T value) {
    return new Node<Type>(r, c, value);
}
int N1, M1, N2, M2, row, col, Mat1_size = 0, Mat2_size = 0, MatR_size = 0,
                              flag = 1;
void Array_Trans();
template <typename T>
void LinkedList_Trans(T);

Node<Type>*head1 = NULL, *head2 = NULL, *Temp_node, *Res_head = NULL;

template <typename T>
void InsertToLL(Node<T>* node) {
    Node<T>* Temp = node;
    while (Temp->next != NULL) Temp = Temp->next;
    Temp->next = Temp_node;
}
void print_Trans_Mat() {
    //---------------------------------------\ Print the Matrix Transpose Result
    ///---------------------------
    // cout<<"------------Matrix after transpose---------"<<endl;

    int Rindex = 0;
    for (int i = 0; i < M1; i++) {
        for (int j = 0; j < N1; j++) {
            if (Rindex < MatR_size && Res_Mat[Rindex][0] == i &&
                Res_Mat[Rindex][1] == j) {
                cout << Res_Mat[Rindex++][2] << " ";
            } else
                cout << "0 ";
        }
        cout << endl;
    }
}

/********************************************************| SPARSE MATRIX
 * MULTIPLICATION |************************************************/
void Array_Mult() {

    cin >> N2 >> M2;  // Input for Matrix 2..
    for (int i = 0; i < N2; i++) {
        for (int j = 0; j < M2; j++) {
            cin >> value;
            if (value != 0) {
                Matrix2[Mat2_size][0] = i;
                Matrix2[Mat2_size][1] = j;
                Matrix2[Mat2_size][2] = value;
                Mat2_size++;
            }
        }
    }
    flag = 0;
    Array_Trans();
    flag = 1;
    Mat1_size = 0;
    for (int i = 0; i < Mat2_size; i++) {
        Matrix1[i][0] = Matrix2[i][0];
        Matrix1[i][1] = Matrix2[i][1];
        Matrix1[i][2] = Matrix2[i][2];
        Mat1_size++;
    }
    Mat2_size = 0;
    for (int i = 0; i < MatR_size; i++) {
        Matrix2[i][0] = Res_Mat[i][0];
        Matrix2[i][1] = Res_Mat[i][1];
        Matrix2[i][2] = Res_Mat[i][2];
        Mat2_size++;
    }
    // int k=N1;
    //     N1=N2;
    //     N2=M1;
    //     M1=M2;
    //     M2=k;

    // First Matrix is N1xM1;

    //-----------------------------------\ MULTIPLICATION OF TWO SPARSE MATRIX
    ///----------------------------
    int Curr_row, Curr_col;
    MatR_size = 0;
    for (Curr_row = 0; Curr_row < Mat1_size;) {
        int r = Matrix1[Curr_row][0];
        for (Curr_col = 0; Curr_col < Mat2_size;) {
            // cout<<Curr_col<<"<-C-COL  ";
            int c = Matrix2[Curr_col][0];
            int Temp1 = Curr_row;
            int Temp2 = Curr_col;
            Sum = 0;
            while (Temp1 < Mat1_size && Matrix1[Temp1][0] == r &&
                   Temp2 < Mat2_size && Matrix2[Temp2][0] == c) {
                if (Matrix1[Temp1][1] > Matrix2[Temp2][1])
                    Temp2++;
                else if (Matrix1[Temp1][1] < Matrix2[Temp2][1])
                    Temp1++;
                else {
                    Sum += Matrix1[Temp1++][2] * Matrix2[Temp2++][2];
                }
            }
            if (Sum != 0) {
                Res_Mat[MatR_size][0] = r;
                Res_Mat[MatR_size][1] = c;
                Res_Mat[MatR_size++][2] = Sum;
            }
            while (Curr_col < Mat2_size && Matrix2[Curr_col][0] == c)
                Curr_col++;
        }
        while (Curr_row < Mat1_size && Matrix1[Curr_row][0] == r) Curr_row++;
    }

    //---------------------------------------\ Print the Matrix Multiplication
    // Result /--------------------------- cout<<"Matrix Result as sparse
    // -----"<<endl;
    int Rindex = 0;
    for (int i = 0; i < N2; i++) {
        for (int j = 0; j < M1; j++) {
            if (Rindex < MatR_size && Res_Mat[Rindex][0] == i &&
                Res_Mat[Rindex][1] == j) {
                cout << Res_Mat[Rindex++][2] << " ";
            } else
                cout << "0 ";
        }
        cout << endl;
    }
}

/********************************************************| SPARSE MATRIX
 * ADDITION |*****************************************************/

void Array_Add() {
    cin >> N1 >> M1;  // Input for Matrix 1..
    for (int i = 0; i < N1; i++) {
        for (int j = 0; j < M1; j++) {
            cin >> value;
            if (value != 0) {
                Matrix1[Mat1_size][0] = i;
                Matrix1[Mat1_size][1] = j;
                Matrix1[Mat1_size][2] = value;
                Mat1_size++;
            }
        }
    }
    cin >> N2 >> M2;  // Input for Matrix 2..
    for (int i = 0; i < N2; i++) {
        for (int j = 0; j < M2; j++) {
            cin >> value;
            if (value != 0) {
                Matrix2[Mat2_size][0] = i;
                Matrix2[Mat2_size][1] = j;
                Matrix2[Mat2_size][2] = value;
                Mat2_size++;
            }
        }
    }
    //-----------------------------------\ ADDITION OF TWO SPARSE MATRIX
    ///----------------------------
    int len1 = 0, len2 = 0;
    // cout<<endl<<" Mat1 : "<<<<"Mat2 : "<<<<endl;
    while (len1 < Mat1_size && len2 < Mat2_size) {
        if (Matrix1[len1][0] > Matrix2[len2][0] ||
            Matrix1[len1][0] == Matrix2[len2][0] &&
                Matrix1[len1][1] > Matrix2[len2][1]) {
            Res_Mat[MatR_size][0] = Matrix2[len2][0];
            Res_Mat[MatR_size][1] = Matrix2[len2][1];
            Res_Mat[MatR_size][2] = Matrix2[len2][2];
            len2++;

        } else if (Matrix1[len1][0] < Matrix2[len2][0] ||
                   Matrix1[len1][0] == Matrix2[len2][0] &&
                       Matrix1[len1][1] < Matrix2[len2][1]) {
            Res_Mat[MatR_size][0] = Matrix1[len1][0];
            Res_Mat[MatR_size][1] = Matrix1[len1][1];
            Res_Mat[MatR_size][2] = Matrix1[len1][2];
            len1++;
        } else {

            if ((Matrix1[len1][2] + Matrix2[len2][2]) != 0) {
                Res_Mat[MatR_size][0] = Matrix2[len2][0];
                Res_Mat[MatR_size][1] = Matrix2[len2][1];
                Res_Mat[MatR_size][2] = Matrix1[len1][2] + Matrix2[len2][2];
            } else
                MatR_size--;
            len2++;
            len1++;
        }
        MatR_size++;
    }
    while (len1 < Mat1_size) {
        Res_Mat[MatR_size][0] = Matrix1[len1][0];
        Res_Mat[MatR_size][1] = Matrix1[len1][1];
        Res_Mat[MatR_size][2] = Matrix1[len1][2];
        len1++;
        MatR_size++;
    }
    while (len2 < Mat2_size) {
        Res_Mat[MatR_size][0] = Matrix2[len2][0];
        Res_Mat[MatR_size][1] = Matrix2[len2][1];
        Res_Mat[MatR_size][2] = Matrix2[len2][2];
        len2++;
        MatR_size++;
    }
    //---------------------------------------\ Print the Matrix Addition Result
    ///--------------------------- cout<<"------------Matrix after
    // addition---------"<<endl;
    int Rindex = 0;
    for (int i = 0; i < N1; i++) {
        for (int j = 0; j < M1; j++) {
            if (Rindex < MatR_size && Res_Mat[Rindex][0] == i &&
                Res_Mat[Rindex][1] == j) {
                cout << Res_Mat[Rindex++][2] << " ";
            } else
                cout << "0 ";
        }
        cout << endl;
    }
}

/********************************************************| SPARSE MATRIX
 * TRANSPOSE |*****************************************************/
void Array_Trans() {
    cin >> N1 >> M1;  // Input for Matrix 1..
    for (int i = 0; i < N1; i++) {
        for (int j = 0; j < M1; j++) {
            cin >> value;
            if (value != 0) {
                Matrix1[Mat1_size][0] = i;
                Matrix1[Mat1_size][1] = j;
                Matrix1[Mat1_size][2] = value;
                Mat1_size++;
            }
        }
    }

    int* Col_Count =
        new int[M1 + 1];  //-->To store total number of elements in each row...

    for (int i = 0; i < M1; i++) Col_Count[i] = 0;
    int* ind = new int[M1 + 1];  //-->To store total number of elements will be
                                 // before a row col value,after transpose.
    ind[0] = 0;

    for (int i = 0; i < Mat1_size; i++) Col_Count[(int)Matrix1[i][1]]++;

    for (int i = 1; i <= M1; i++) ind[i] = ind[i - 1] + Col_Count[i - 1];

    for (int i = 0; i < Mat1_size; i++) {
        int pos = ind[(int)Matrix1[i][1]]++;
        Res_Mat[pos][0] = Matrix1[i][1];
        Res_Mat[pos][1] = Matrix1[i][0];
        Res_Mat[pos][2] = Matrix1[i][2];
        MatR_size++;
    }

    if (flag) print_Trans_Mat();
}

template <typename T>
void LinkedList_Mult(T tmp) {
    flag = 0;
    cin >> N1 >> M1;  // Input for Matrix 1..
    for (int i = 0; i < N1; i++) {
        for (int j = 0; j < M1; j++) {
            cin >> value;
            if (value != 0) {
                Temp_node = create_node(i, j, value);
                if (head1 == NULL)
                    head1 = Temp_node;
                else
                    InsertToLL(head1);
            }
        }
    }
    LinkedList_Trans(tmp);
    head2 = Res_head->next;
    Res_head = NULL;

    //-----------------------------------\ MULTIPLICATION OF TWO SPARSE MATRIX
    // in LinkedList/----------------------------

    Node<T>*Temp_M1 = head1, *Temp_M2 = head2, *Temp_r = NULL;
    while (Temp_M1 != NULL) {
        int r = Temp_M1->row;
        Temp_M2 = head2;
        while (Temp_M2 != NULL) {
            int c = Temp_M2->row;
            Node<T>* Temp1 = Temp_M1;
            Node<T>* Temp2 = Temp_M2;
            Sum = 0;
            while (Temp1 != NULL && Temp1->row == r && Temp2 != NULL &&
                   Temp2->row == c) {
                if (Temp1->col > Temp2->col)
                    Temp2 = Temp2->next;
                else if (Temp1->col < Temp2->col)
                    Temp1 = Temp1->next;
                else {
                    Sum += Temp1->value * Temp2->value;
                    Temp2 = Temp2->next;
                    Temp1 = Temp1->next;
                }
            }
            if (Sum != 0) {
                Temp_node = create_node(r, c, Sum);
                if (Temp_r == NULL) {
                    Temp_r = Temp_node;
                    Res_head = Temp_node;
                }
                Temp_r->next = Temp_node;
                Temp_r = Temp_node;
            }
            while (Temp_M2 != NULL && Temp_M2->row == c)
                Temp_M2 = Temp_M2->next;
        }
        while (Temp_M1 != NULL && Temp_M1->row == r) Temp_M1 = Temp_M1->next;
    }

    //---------------------------------------\ Print the Matrix Mulitplication
    // Result from LinkedList /---------------------------
    // cout<<"------------Matrix after multiplication---------"<<endl;
    Node<T>* Temp = Res_head;
    for (int i = 0; i < N1; i++) {
        for (int j = 0; j < M1; j++) {
            if (Temp != NULL && Temp->row == i && Temp->col == j) {
                cout << Temp->value << " ";
                Temp = Temp->next;
            } else
                cout << "0 ";
        }
        cout << endl;
    }
}

template <typename T>
void LinkedList_Add(T tmp) {
    cin >> N1 >> M1;  // Input for Matrix 1..
    for (int i = 0; i < N1; i++) {
        for (int j = 0; j < M1; j++) {
            cin >> value;
            if (value != 0) {
                Temp_node = create_node(i, j, value);
                if (head1 == NULL)
                    head1 = Temp_node;
                else
                    InsertToLL(head1);
            }
        }
    }
    cin >> N2 >> M2;  // Input for Matrix 2..
    for (int i = 0; i < N2; i++) {
        for (int j = 0; j < M2; j++) {
            cin >> value;
            if (value != 0) {
                Temp_node = create_node(i, j, value);
                if (head2 == NULL)
                    head2 = Temp_node;
                else
                    InsertToLL(head2);
            }
        }
    }

    Node<T>*Temp_M1 = head1, *Temp_M2 = head2, *Temp_r = NULL;
    while (Temp_M1 != NULL && Temp_M2 != NULL) {
        if (Temp_M1->row > Temp_M2->row ||
            Temp_M1->row == Temp_M2->row && Temp_M1->col > Temp_M2->col) {
            Temp_node = create_node(Temp_M2->row, Temp_M2->col, Temp_M2->value);
            if (Res_head == NULL)
                Res_head = Temp_node;
            else
                InsertToLL(Res_head);
            Temp_M2 = Temp_M2->next;
        } else if (Temp_M1->row < Temp_M2->row ||
                   Temp_M1->row == Temp_M2->row &&
                       Temp_M1->col < Temp_M2->col) {
            Temp_node = create_node(Temp_M1->row, Temp_M1->col, Temp_M1->value);
            if (Res_head == NULL)
                Res_head = Temp_node;
            else
                InsertToLL(Res_head);
            Temp_M1 = Temp_M1->next;
        } else {
            if ((Temp_M1->value + Temp_M2->value) != 0) {
                Temp_node = create_node(Temp_M1->row, Temp_M1->col,
                                        Temp_M1->value + Temp_M2->value);
                if (Res_head == NULL)
                    Res_head = Temp_node;
                else
                    InsertToLL(Res_head);
            }
            Temp_M2 = Temp_M2->next;
            Temp_M1 = Temp_M1->next;
        }
    }

    //---------------------------------------\ Print the Matrix Addition Result
    // from LinkedList /---------------------------
    cout << "------------Matrix after addition---------" << endl;
    Node<T>* Temp = Res_head;
    for (int i = 0; i < N1; i++) {
        for (int j = 0; j < M1; j++) {
            if (Temp != NULL && Temp->row == i && Temp->col == j) {
                cout << Temp->value << " ";
                Temp = Temp->next;
            } else
                cout << "0 ";
        }
        cout << endl;
    }
}

template <typename T>
void LinkedList_Trans(T tmp) {
    cin >> N2 >> M2;  // Input for Matrix 2..
    for (int i = 0; i < N2; i++) {
        for (int j = 0; j < M2; j++) {
            cin >> value;
            if (value != 0) {
                Temp_node = create_node(i, j, value);
                if (head2 == NULL)
                    head2 = Temp_node;
                else
                    InsertToLL(head2);
            }
        }
    }
    Node<T>*Temp = head2, *Temp_node_prev;
    Res_head = create_node(-1, -1, -1);
    while (Temp != NULL) {
        Temp_node = Res_head->next;
        if (Temp_node == NULL) {
            Res_head->next = create_node(Temp->col, Temp->row, Temp->value);
        }
        Temp_node_prev = Res_head;
        while (Temp_node != NULL &&
               (Temp->col > Temp_node->row ||
                Temp->col == Temp_node->row && Temp->row > Temp_node->col)) {
            Temp_node_prev = Temp_node;
            Temp_node = Temp_node->next;
        }
        // cout<<" value--"<<Temp->value<<endl;
        Temp_node_prev->next = create_node(Temp->col, Temp->row, Temp->value);
        Temp_node_prev->next->next = Temp_node;
        Temp = Temp->next;
    }

    if (flag) {

        // cout<<"------------Matrix after Transpose---------"<<endl;
        Temp = Res_head->next;
        for (int i = 0; i < N2; i++) {
            for (int j = 0; j < M2; j++) {
                if (Temp != NULL && Temp->row == i && Temp->col == j) {
                    cout << Temp->value << " ";
                    Temp = Temp->next;
                } else
                    cout << "0 ";
            }
            cout << endl;
        }
    }
}

int main() {
    // cout<<"Well come to Array and Linked List"<<endl;
    cin >> choice1 >> choice2;
    if (choice1 == 1) {  // Option:1 for Array operation..s
        switch (choice2) {
            case 1:
                Array_Add();  // 1 for Addition...
                break;
            case 2:
                Array_Trans();  // 2 for transpose...
                break;
            case 3:
                Array_Mult();  // 3 for multiplication....
                break;
            default:
                cout << "Invailid input!";
        }
    } else if (choice1 == 2) {  // Option:2 for Linked List operations
        switch (choice2) {
            case 1:
                LinkedList_Add(k);  // 1 for Addition...
                break;
            case 2:
                LinkedList_Trans(k);  // 2 for transpose...
                break;
            case 3:
                flag = 0;
                LinkedList_Mult(k);  // 3 for multiplication....
                break;
            default:
                cout << "Invailid input!";
        }
    } else {
        cout << "Invailid input!";
    }
    return 0;
}

// int main()
// {
// 	cout<<"hellooooooooooo"<<endl;
// 	Matrix obj;
// 	//cout<<"Well come to Array and Linked List"<<endl;
// 	cin>>choice1>>choice2;
// 	if(choice1==1){                              //Option:1 for Array
// operation..s
//       switch(choice2){
//       	case 1: obj.Array_Add();                          //1 for
//       Addition... 	       break; 	case 2: obj.Array_Trans();
//       //2 for transpose... 	       break; 	case 3: obj.Array_Mult(); //3
//       for multiplication.... 	       break; 	default:cout<<"Invailid
//       input!";
//       }
// 	}
// 	else if(choice1==2){					     //Option:2
// for Linked List operations 		switch(choice2){
//       	case 1: obj.LinkedList_Add();                    //1 for
//       Addition... 	       break; 	case 2:obj.LinkedList_Trans(); //2 for
//       transpose... 	       break; 	case 3:
//       obj.LinkedList_Mult(); //3 for multiplication.... 	       break;
//       default:cout<<"Invailid input!";
// 	}
// }
// 	else
// 	{
// 		cout<<"Invailid input!";
// 	}
// 	return 0;
// }
