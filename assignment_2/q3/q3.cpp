#include <string.h>
#include <cstring>
#include <iostream>

// --------------- Define Comparator --------------------

/*
Syntax to write comparator
int cmprtr(typename a, typename b)
{
    if(greater)
        return 0;
    if(smaller)
        return 1;
    if(equals ==)
        return 2;
}
*/
template <typename T>
int cmprtr(T &x, T &y) {
    if (x == y)
        return 2;
    else if (x < y)
        return 1;
    else
        return 0;
}

int cmprtr(char *x, char *y) {
    if (strcmp(x, y) < 0)
        return 1;
    else if (strcmp(x, y) == 0)
        return 2;
    else
        return 0;
}

// ---------------- Declare Custom Class
/*
Syntax for custom class
class Name{};

-- Note that overloading << operator is compulsary as to print I have used cout
in a lot of functions

std::ostream &operator<<(std::ostream &out, MyClass &c) {
    out << c.a << " " << c.b;
    return out;
}
*/

class MyClass {
   public:
    int a;
    int b;
    MyClass() {
        a = 0;
        b = 0;
    }
    MyClass(int _a, int _b) {
        a = _a;
        b = _b;
    }
};

std::ostream &operator<<(std::ostream &out, MyClass &c) {
    out << c.a << " " << c.b;
    return out;
}

int cmprtr(MyClass a, MyClass b) {
    if (a.a < b.a)
        return 1;
    else if (a.a == b.a)
        return 2;
    else
        return 0;
}

// ---------------Data Type------------------

template <class T>
class AVLTree {

   private:
    //    -------------- Data Type --------------
    struct Node {
        T value;
        Node *left;
        Node *right;
        int height;
        int count;
        int numright;
        int numleft;

        Node(T key) {
            value = key;
            left = NULL;
            right = NULL;
            height = 1;
            count = 1;
            numright = 0;
            numleft = 0;
        }
    };

    //    Members
    Node *root;

    // Methods

    // Utility to get maximum
    int max(int a, int b) { return a > b ? a : b; }

    // Utility to get height of a node from it's children
    int getHeight(Node *node) {
        if (node == NULL) return 0;
        return node->height;
    }

    // Utility to set height of a node
    void setHeight(Node *node) {
        if (node == NULL) return;
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    }

    // Utility to get balance factor using left and right height
    int getBF(Node *node) {
        if (node == NULL) return 0;

        return getHeight(node->left) - getHeight(node->right);
    }

    int getCount(Node *node) {
        if (node == NULL) return 0;
        return node->numright + node->numleft;
    }

    // Utility for left rotation and update height
    Node *left_rotate(Node *node) {
        Node *temp = node->right;
        node->right = temp->left;
        temp->left = node;

        node->numright = temp->numleft;
        temp->numleft = 1 + node->numleft + node->numright;
        setHeight(node);
        setHeight(temp);

        return temp;
    }

    // Utitlity of right rotation and update height
    Node *right_rotate(Node *node) {
        Node *temp = node->left;
        node->left = temp->right;
        temp->right = node;

        node->numleft = temp->numright;
        temp->numright = 1 + node->numleft + node->numright;
        setHeight(node);
        setHeight(temp);

        return temp;
    }

    // Utility to rebalance a node
    Node *rebalance(Node *node, int balancefac) {
        // LL Case
        if (balancefac == 2 and getBF(node->left) > 0) {
            node = right_rotate(node);
            // LR Case
        } else if (balancefac == 2 and getBF(node->left) <= 0) {
            node->left = left_rotate(node->left);
            node = right_rotate(node);
            // RR Case
        } else if (balancefac == -2 and getBF(node->right) <= 0) {
            node = left_rotate(node);
            // RL Case
        } else if (balancefac == -2 and getBF(node->right) > 0) {
            node->right = right_rotate(node->right);
            node = left_rotate(node);
        }

        return node;
    }

    // Utility to find Highest element in the left subtree
    Node *inorderPredecessor(Node *node) {
        Node *temp = node;
        while (temp->right != NULL) temp = temp->right;
        return temp;
    }

    // Utitlity to insert a node in the tree
    Node *insertHelper(Node *node, T key) {
        if (node == NULL) {
            node = new Node(key);
            return node;
        }

        if (cmprtr(node->value, key) == 0) {
            node->left = insertHelper(node->left, key);
            node->numleft++;
        } else if (cmprtr(node->value, key) == 1) {
            node->right = insertHelper(node->right, key);
            node->numright++;
        } else {
            node->count++;
            node->numright++;
        }

        setHeight(node);

        node = rebalance(node, getBF(node));

        return node;
    }

    // Utitlity to delete a node in the tree
    Node *deleteHelper(Node *node, T key, int &todel) {
        if (node == NULL) return node;

        if (cmprtr(node->value, key) == 0) {
            node->left = deleteHelper(node->left, key, todel);
            node->numleft -= todel;
        } else if (cmprtr(node->value, key) == 1) {
            node->right = deleteHelper(node->right, key, todel);
            node->numright -= todel;
        } else {
            if (node->left == NULL and node->right == NULL) {
                todel = node->count;
                delete node;
                node = NULL;
            }

            // One right child
            else if (node->left == NULL) {
                Node *temp = node->right;
                todel = node->count;
                delete node;
                node = temp;
            }

            // One left child
            else if (node->right == NULL) {
                Node *temp = node->left;
                todel = node->count;
                delete node;
                node = temp;
            }
            // Node with both children
            else {
                Node *inorderP = inorderPredecessor(node->left);
                todel = node->count;

                node->value = inorderP->value;
                node->count = inorderP->count;

                node->left = deleteHelper(node->left, inorderP->value, todel);
                node->numleft -= todel;
            }
        }

        setHeight(node);

        node = rebalance(node, getBF(node));
        return node;
    }

    // Utility to search a node in the tree
    bool searchHelper(Node *node, T key) {
        if (node == NULL) return false;

        if (cmprtr(node->value, key) == 2) return true;

        if (cmprtr(node->value, key) == 0) return searchHelper(node->left, key);
        if (cmprtr(node->value, key) == 1)
            return searchHelper(node->right, key);

        return false;
    }

    // Utility to count occurence of a key
    int countOccurenceHelper(Node *node, T key) {
        if (node == NULL) return 0;

        if (cmprtr(node->value, key) == 2) return node->count;

        if (cmprtr(node->value, key) == 0)
            return countOccurenceHelper(node->left, key);
        if (cmprtr(node->value, key) == 1)
            return countOccurenceHelper(node->right, key);

        return 0;
    }

    // Utility to find kth largest node
    T kthLargestHelper(Node *node, int k) {
        if (node == NULL) return T();

        Node *temp = node;
        while (temp) {
            if (k == temp->numright + 1 or
                (!temp->right and temp->numright > 0 and k <= temp->numright)) {
                return temp->value;
            } else if (k < temp->numright + 1) {
                if (temp->count > 1 and
                    (k == temp->numright + 1 - temp->count + 1))
                    return temp->value;
                temp = temp->right;
            } else {
                k = k - temp->numright - 1;
                temp = temp->left;
            }
        }
        return T();
    }

    // Utility to find number of nodes in a range
    int countRangeHelper(Node *node, T eLeft, T eRight) {
        if (node == NULL) return 0;

        if ((cmprtr(node->value, eLeft) == 0 or
             cmprtr(node->value, eLeft) == 2) and
            (cmprtr(node->value, eRight) == 1 or
             cmprtr(node->value, eRight) == 2))
            return 1 + countRangeHelper(node->left, eLeft, eRight) +
                   countRangeHelper(node->right, eLeft, eRight);
        else if ((cmprtr(node->value, eLeft) == 0 or
                  cmprtr(node->value, eLeft) == 2))
            return countRangeHelper(node->left, eLeft, eRight);
        else
            return countRangeHelper(node->right, eLeft, eRight);
    }

    // Utility to delete the whole tree in order keep memory clean
    void deleteTree(Node *node) {
        if (node == NULL) return;

        deleteTree(node->left);
        deleteTree(node->right);

        delete node;
    }

   public:
    AVLTree() { root = NULL; }
    ~AVLTree() { deleteTree(root); }
    Node *getRoot() { return root; }
    void insert(T key) { root = insertHelper(root, key); }
    void delete_node(T key) {
        int todel = 0;
        root = deleteHelper(root, key, todel);
    }
    bool search(T key) { return searchHelper(root, key); }
    int count_occurence(T key) { return countOccurenceHelper(root, key); }
    T lower_bound(T n) {
        Node *curr = root;
        T *ans = nullptr;

        while (curr != NULL) {
            if (cmprtr(curr->value, n) == 0) {
                if (ans == nullptr or cmprtr(*ans, curr->value) == 0)
                    *ans = curr->value;
                curr = curr->left;

            } else if (cmprtr(curr->value, n) == 1) {
                curr = curr->right;

            } else {
                return curr->value;
            }
        }
        return ans == nullptr ? T() : *ans;
    }
    T upper_bound(T n) {
        Node *curr = root;
        T *ans = nullptr;

        while (curr != NULL) {
            if (cmprtr(curr->value, n) == 0) {
                if (ans == nullptr or cmprtr(*ans, curr->value) == 0)
                    ans = &curr->value;
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }
        return ans == nullptr ? T() : *ans;
    }
    int closest_element(int n) {
        Node *curr = root;
        Node *prev = NULL;
        int diff = INT32_MAX;
        int ans = 0;

        while (curr != NULL) {
            if (curr->value > n) {
                if (abs(curr->value - n) < diff) {
                    diff = abs(curr->value - n);
                    ans = curr->value;
                }
                curr = curr->left;
            } else if (curr->value < n) {
                if (abs(curr->value - n) < diff) {
                    diff = abs(curr->value - n);
                    ans = curr->value;
                }
                curr = curr->right;
            } else {
                return curr->value;
            }
        }
        return ans;
    }
    T Kth_largest(int k) { return kthLargestHelper(root, k); }
    int count_range(T eLeft, T eRight) {
        return countRangeHelper(root, eLeft, eRight);
    }
    void inorder(Node *root) {
        if (root == NULL) return;

        inorder(root->left);
        int i = root->count;
        while (i--) std::cout << root->value << "  ";
        inorder(root->right);
    }
    void preorder(Node *root) {
        if (root == NULL) return;

        int i = root->count;
        while (i--) std::cout << root->value << "  ";
        preorder(root->left);
        preorder(root->right);
    }

    void printBT(const std::string &prefix, Node *node, bool isLeft) {
        if (node != nullptr) {
            std::cout << prefix;

            std::cout << (isLeft ? "├──" : "└──");

            // print the value of the node
            std::cout << node->value << std::endl;

            // enter the next tree level - left and right branch
            printBT(prefix + (isLeft ? "│   " : "    "), node->left, true);
            printBT(prefix + (isLeft ? "│   " : "    "), node->right, false);
        }
    }

    void printBT(Node *node) { printBT("", node, false); }
};

// ----------------- Driver Code ------------------------

int main() {
    AVLTree<MyClass> b;

    // b.insert(8);
    // b.insert(7);
    // b.insert(6);
    // b.insert(5);
    // b.insert(4);
    // b.insert(3);
    // b.insert(2);
    // b.insert(1);
    // b.insert(5);
    // b.insert(6);
    // b.insert(6);
    // b.insert(3);
    // b.insert(54);
    // b.insert(59);
    // b.insert(65);
    // b.insert(70);
    // b.insert(51);
    // b.insert(5);
    // b.insert(10);
    // b.insert(5);
    // b.insert(9);
    // b.insert(7);
    // b.insert(17);

    // std::string str = "hello";
    // std::string str1 = "bye";
    // std::string str2 = "bhanu";
    // std::string str3 = "king";
    // std::string str4 = "gandhi";
    // std::string str5 = "yellow";
    // std::string str6 = "white";
    // std::string str7 = "white";
    // std::string str8 = "white";
    // std::string str9 = "white";
    // std::string str10 = "white";
    // std::string str11 = "white";
    // std::string str12 = "white";
    // std::string str13 = "white";

    MyClass a(1, 2);
    MyClass k(3, 2);
    MyClass c(4, 2);
    MyClass d(5, 2);
    MyClass e(6, 2);
    MyClass f(7, 2);
    MyClass g(8, 2);
    MyClass h(9, 2);
    MyClass z(10, 2);
    // int a = 1;
    // int c = 2;
    // int d = 3;
    // int e = 4;
    // int f = 5;
    // int g = 6;
    // int h = 7;
    // int i = 8;
    // int j = 9;
    // int k = 10;
    // int l = 11;
    b.insert(a);
    b.insert(k);
    b.insert(c);
    b.insert(d);
    b.insert(e);
    b.insert(f);
    b.insert(g);
    b.insert(h);
    // b.insert(i);
    // b.insert(j);
    // b.insert(k);
    // b.insert(l);
    // b.insert(l);
    // b.insert(l);
    // std::cout << b.lower_bound(8) << std::endl;
    // std::cout << b.lower_bound(61) << std::endl;
    // b.delete_node(str5);

    b.inorder(b.getRoot());
    std::cout << std::endl;

    std::cout << b.upper_bound(e).a << std::endl;

    std::cout << "--------------------------------" << std::endl;
    b.printBT(b.getRoot());

    return 0;
}