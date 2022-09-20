#include <iostream>

struct Node {
    int value;
    Node *left;
    Node *right;
    int height;
    int count;

    Node(int key) {
        value = key;
        left = NULL;
        right = NULL;
        height = 1;
        count = 1;
    }
};

class AVLTree {
   private:
    //    Members
    Node *root;

    // Methods

    // Utility to get maximum
    int max(int a, int b);

    // Utility to get height of a node from it's children
    int getHeight(Node *node);

    // Utility to set height of a node
    void setHeight(Node *node);

    // Utility to get balance factor using left and right height
    int getBF(Node *node);

    // Utility for left rotation and update height
    Node *left_rotate(Node *node);

    // Utitlity of right rotation and update height
    Node *right_rotate(Node *node);

    // Utility to rebalance a node
    Node *rebalance(Node *node, int bf);

    // Utility to find Highest element in the left subtree
    Node *inorderPredecessor(Node *node);

    // Utitlity to insert a node in the tree
    Node *insertHelper(Node *node, int key);

    // Utitlity to delete a node in the tree
    Node *deleteHelper(Node *node, int key);

    // Utility to search a node in the tree
    bool searchHelper(Node *node, int key);

    // Utility to count occurence of a key
    int countOccurenceHelper(Node *node, int key);

    // Utility to find kth largest node
    void kthLargestHelper(Node *node, int &k, int &ans, bool &flag);

    // Utility to find number of nodes in a range
    int countRangeHelper(Node *node, int low, int high);

    // Utility to delete the whole tree in order keep memory clean
    void deleteTree(Node *node);

   public:
    AVLTree();
    ~AVLTree();
    Node *getRoot();
    void insert(int key);
    void delete_node(int key);
    bool search(int key);
    int count_occurence(int key);
    int lower_bound(int n);
    int upper_bound(int n);
    int closest_element(int n);
    int Kth_largest(int k);
    int count_range(int eLeft, int eRight);
    void inorder(Node *root);
    void preorder(Node *root);
};

void printBT(const std::string &prefix, const Node *node, bool isLeft) {
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

void printBT(const Node *node) { printBT("", node, false); }

// --------------------- Private ----------------------

int AVLTree::max(int a, int b) { return a > b ? a : b; }

int AVLTree::getHeight(Node *node) {
    if (node == NULL) return 0;
    return node->height;
}

void AVLTree::setHeight(Node *node) {
    if (node == NULL) return;
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
}

int AVLTree::getBF(Node *node) {
    if (node == NULL) return 0;

    return getHeight(node->left) - getHeight(node->right);
}

Node *AVLTree::left_rotate(Node *node) {
    Node *temp = node->right;
    node->right = temp->left;
    temp->left = node;

    setHeight(node);
    setHeight(temp);

    return temp;
}

Node *AVLTree::right_rotate(Node *node) {
    Node *temp = node->left;
    node->left = temp->right;
    temp->right = node;

    setHeight(node);
    setHeight(temp);

    return temp;
}

Node *AVLTree::rebalance(Node *node, int balancefac) {
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

Node *AVLTree::inorderPredecessor(Node *node) {
    Node *temp = node;
    while (temp->right != NULL) temp = temp->right;
    return temp;
}

Node *AVLTree::insertHelper(Node *node, int key) {
    if (node == NULL) {
        node = new Node(key);
        return node;
    }

    if (node->value > key)
        node->left = insertHelper(node->left, key);
    else if (node->value < key)
        node->right = insertHelper(node->right, key);
    else {
        node->count++;
        return node;
    }

    setHeight(node);

    node = rebalance(node, getBF(node));

    return node;
}

Node *AVLTree::deleteHelper(Node *node, int key) {
    if (node == NULL) return node;

    if (node->value > key)
        node->left = deleteHelper(node->left, key);
    else if (node->value < key)
        node->right = deleteHelper(node->right, key);
    else {
        // if (node->count > 1) {
        //     node->count--;
        //     return node;
        // }
        // No child
        if (node->left == NULL and node->right == NULL) {
            delete node;
            node = NULL;
        }

        // One right child
        else if (node->left == NULL) {
            Node *temp = node->right;
            delete node;
            node = temp;
        }

        // One left child
        else if (node->right == NULL) {
            Node *temp = node->left;
            delete node;
            node = temp;
        }
        // Node with both children
        else {
            Node *inorderP = inorderPredecessor(node->left);

            // Swap values
            // std::swap(node->value, inorderP->value);
            node->value = inorderP->value;
            node->count = inorderP->count - 1;

            node->left = deleteHelper(node->left, inorderP->value);
        }
    }

    setHeight(node);

    node = rebalance(node, getBF(node));
    return node;
}

bool AVLTree::searchHelper(Node *node, int key) {
    if (node == NULL) return false;

    if (node->value == key) return true;

    if (node->value > key) return searchHelper(node->left, key);
    if (node->value < key) return searchHelper(node->right, key);

    return false;
}

int AVLTree::countOccurenceHelper(Node *node, int key) {
    if (node == NULL) return 0;

    if (node->value == key) return node->count;

    if (node->value > key) return countOccurenceHelper(node->left, key);
    if (node->value < key) return countOccurenceHelper(node->right, key);

    return 0;
}

void AVLTree::kthLargestHelper(Node *node, int &k, int &ans, bool &flag) {
    if (flag or node == NULL) return;

    kthLargestHelper(node->right, k, ans, flag);

    k -= node->count;
    if (k <= 0 and flag == false) {
        flag = true;
        ans = node->value;
        return;
    }

    kthLargestHelper(node->left, k, ans, flag);
}

int AVLTree::countRangeHelper(Node *node, int eLeft, int eRight) {
    if (node == NULL) return 0;

    if (node->value >= eLeft and node->value <= eRight)
        return 1 + countRangeHelper(node->left, eLeft, eRight) +
               countRangeHelper(node->right, eLeft, eRight);
    else if (node->value >= eLeft)
        return countRangeHelper(node->left, eLeft, eRight);
    else
        return countRangeHelper(node->right, eLeft, eRight);
}

void AVLTree::deleteTree(Node *node) {
    if (node == NULL) return;

    deleteTree(node->left);
    deleteTree(node->right);

    delete node;
}

// -------------------- Public -------------------
AVLTree::AVLTree() { root = NULL; }

AVLTree::~AVLTree() { deleteTree(root); }

Node *AVLTree::getRoot() { return root; }

void AVLTree::insert(int key) { root = insertHelper(root, key); }

void AVLTree::delete_node(int key) { root = deleteHelper(root, key); }

bool AVLTree::search(int key) { return searchHelper(root, key); }

int AVLTree::count_occurence(int key) {
    return countOccurenceHelper(root, key);
}

int AVLTree::lower_bound(int n) {
    Node *curr = root;
    int ans = INT32_MIN;

    while (curr != NULL) {
        if (curr->value > n) {
            if (ans == INT32_MIN or ans > curr->value) ans = curr->value;
            curr = curr->left;

        } else if (curr->value < n) {
            curr = curr->right;

        } else {
            return curr->value;
        }
    }
    return ans == INT32_MIN ? 0 : ans;
}

int AVLTree::upper_bound(int n) {
    Node *curr = root;
    int ans = INT32_MIN;

    while (curr != NULL) {
        if (curr->value > n) {
            if (ans == INT32_MIN or ans > curr->value) ans = curr->value;
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }
    return ans == INT32_MIN ? 0 : ans;
}

int AVLTree::closest_element(int n) {
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

int AVLTree::Kth_largest(int k) {
    int ans = 0;
    bool flag = false;
    kthLargestHelper(root, k, ans, flag);
    return ans;
}

int AVLTree::count_range(int eLeft, int eRight) {
    return countRangeHelper(root, eLeft, eRight);
}

void AVLTree::inorder(Node *root) {
    if (root == NULL) return;

    inorder(root->left);
    int i = root->count;
    while (i--) std::cout << root->value << "  ";
    inorder(root->right);
}

void AVLTree::preorder(Node *root) {
    if (root == NULL) return;

    int i = root->count;
    while (i--) std::cout << root->value << "  ";
    preorder(root->left);
    preorder(root->right);
}

// ----------------- Driver Code ------------------------

int main() {
    AVLTree b;
    b.insert(8);
    b.insert(7);
    b.insert(6);
    b.insert(5);
    b.insert(4);
    b.insert(3);
    b.insert(2);
    b.insert(1);
    b.insert(5);
    b.insert(6);
    b.insert(6);
    b.insert(3);
    b.insert(54);
    b.insert(59);
    b.insert(65);
    b.insert(70);
    b.insert(51);
    b.insert(5);
    b.insert(10);
    b.insert(5);
    b.insert(9);
    b.insert(7);
    b.insert(17);

    // std::cout << b.upper_bound(61) << std::endl;
    // std::cout << b.lower_bound(61) << std::endl;

    b.inorder(b.getRoot());
    std::cout << std::endl;

    std::cout << b.Kth_largest(15) << std::endl;

    std::cout << "--------------------------------" << std::endl;
    printBT(b.getRoot());

    return 0;
}