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
    Node* insertHelper(Node* node, int key);
    bool searchHelper(Node* node, int key);
    int countOccurenceHelper(Node* node, int key);

   public:
    AVLTree();
    ~AVLTree();
    Node* getRoot();
    void insert(int key);
    void inorder(Node* root);
    void preorder(Node* root);
    bool search(int key);
    int count_occurence(int key);
};

AVLTree::AVLTree() { root = NULL; }

AVLTree::~AVLTree() {}

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

Node* AVLTree::getRoot() { return root; }

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
    int balancefac = getBF(node);

    // If left insertion (LL Case)
    if (balancefac == 2 and key < node->left->value) {
        node = right_rotate(node);
    } else if (balancefac == -2 and key > node->right->value) {
        node = left_rotate(node);
    } else if (balancefac == 2 and key > node->left->value) {
        node->left = left_rotate(node->left);
        // left_rotate(node->left);
        node = right_rotate(node);
    } else if (balancefac == -2 and key < node->right->value) {
        node->right = right_rotate(node->right);
        // right_rotate(root->right);
        node = left_rotate(node);
    }

    return node;
}

void AVLTree::insert(int key) { root = insertHelper(root, key); }

bool AVLTree::searchHelper(Node* node, int key) {
    if (node == NULL) return false;

    if (node->value == key) return true;

    if (node->value > key) return searchHelper(node->left, key);
    if (node->value < key) return searchHelper(node->right, key);

    return false;
}

bool AVLTree::search(int key) { return searchHelper(root, key); }

int AVLTree::countOccurenceHelper(Node* node, int key) {
    if (node == NULL) return 0;

    if (node->value == key) return node->count;

    if (node->value > key) return countOccurenceHelper(node->left, key);
    if (node->value < key) return countOccurenceHelper(node->right, key);

    return 0;
}

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

int main() {
    AVLTree b;
    b.insert(7);
    b.insert(6);
    b.insert(5);
    b.insert(4);
    b.insert(3);
    b.insert(2);
    b.insert(4);
    b.insert(2);
    b.insert(1);
    b.insert(6);
    b.insert(6);
    b.insert(6);
    b.insert(6);
    b.insert(6);
    std::cout << b.search(1) << std::endl;
    std::cout << b.search(4) << std::endl;
    std::cout << b.count_occurence(4) << std::endl;
    std::cout << b.count_occurence(20) << std::endl;
    std::cout << b.count_occurence(6) << std::endl;

    b.preorder(b.getRoot());
    std::cout << std::endl;
    b.inorder(b.getRoot());
    std::cout << std::endl;

    return 0;
}