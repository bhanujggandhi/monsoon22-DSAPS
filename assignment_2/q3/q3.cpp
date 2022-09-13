#include <iostream>

struct Node {
    int value;
    Node* left;
    Node* right;
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
    Node* root;

    // Methods
    int max(int a, int b);
    int getHeight(Node* node);
    void setHeight(Node* node);
    int getBF(Node* node);
    Node* left_rotate(Node* node);
    Node* right_rotate(Node* node);
    Node* rebalance(Node* node, int bf);
    Node* insertHelper(Node* node, int key);
    bool searchHelper(Node* node, int key);
    int countOccurenceHelper(Node* node, int key);
    void deleteTree(Node* node);

   public:
    AVLTree();
    ~AVLTree();
    Node* getRoot();
    void insert(int key);
    bool search(int key);
    int count_occurence(int key);
    void inorder(Node* root);
    void preorder(Node* root);
};

// --------------------- Private ----------------------

int AVLTree::max(int a, int b) { return a > b ? a : b; }

int AVLTree::getHeight(Node* node) {
    if (node == NULL) return 0;
    return node->height;
}

void AVLTree::setHeight(Node* node) {
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
}

int AVLTree::getBF(Node* node) {
    if (node == NULL) return 0;

    return getHeight(node->left) - getHeight(node->right);
}

Node* AVLTree::left_rotate(Node* node) {
    Node* temp = node->right;
    node->right = temp->left;
    temp->left = node;

    setHeight(node);
    setHeight(temp);

    return temp;
}

Node* AVLTree::right_rotate(Node* node) {
    Node* temp = node->left;
    node->left = temp->right;
    temp->right = node;

    setHeight(node);
    setHeight(temp);

    return temp;
}

Node* AVLTree::rebalance(Node* node, int balancefac) {
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

Node* AVLTree::insertHelper(Node* node, int key) {
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

bool AVLTree::searchHelper(Node* node, int key) {
    if (node == NULL) return false;

    if (node->value == key) return true;

    if (node->value > key) return searchHelper(node->left, key);
    if (node->value < key) return searchHelper(node->right, key);

    return false;
}

int AVLTree::countOccurenceHelper(Node* node, int key) {
    if (node == NULL) return 0;

    if (node->value == key) return node->count;

    if (node->value > key) return countOccurenceHelper(node->left, key);
    if (node->value < key) return countOccurenceHelper(node->right, key);

    return 0;
}

void AVLTree::deleteTree(Node* node) {
    if (node == NULL) return;

    deleteTree(node->left);
    deleteTree(node->right);

    delete node;
}

// -------------------- Public -------------------
AVLTree::AVLTree() { root = NULL; }

AVLTree::~AVLTree() { deleteTree(root); }

Node* AVLTree::getRoot() { return root; }

void AVLTree::insert(int key) { root = insertHelper(root, key); }

bool AVLTree::search(int key) { return searchHelper(root, key); }

int AVLTree::count_occurence(int key) {
    return countOccurenceHelper(root, key);
}

void AVLTree::inorder(Node* root) {
    if (root == NULL) return;

    inorder(root->left);
    std::cout << root->value << "  ";
    inorder(root->right);
}

void AVLTree::preorder(Node* root) {
    if (root == NULL) return;

    std::cout << root->value << "  ";
    preorder(root->left);
    preorder(root->right);
}

// ----------------- Driver Code ------------------------

int main() {
    AVLTree b;
    b.insert(7);
    b.insert(6);
    b.insert(5);
    b.insert(4);
    b.insert(3);
    b.insert(2);
    b.insert(1);

    b.preorder(b.getRoot());
    std::cout << std::endl;
    b.inorder(b.getRoot());
    std::cout << std::endl;

    return 0;
}