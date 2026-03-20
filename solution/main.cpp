#include <iostream>

using namespace std;

struct Node {
    int value;
    Node *left;
    Node *right;
    Node(int value): value(value), left(nullptr), right(nullptr){}
};


class Tree{
    private:
    Node *root;
    Node *CopyTree(Node *root){
        if (!root){
            return nullptr;
        }
        Node* NewNode = new Node (root -> value);
        root -> left = CopyTree(root -> left);
        root -> right = CopyTree(root -> right);
        return NewNode;

    }
    Node *DeleteTree(Node *root){
        if (!root){
            return;
        }
        DeleteTree(root -> right);
        DeleteTree(root -> left);
        delete root;
    }
    void PrintNode(Node *root){
        if(!root){
            return;
        }
        cout << root -> value << " ";
        PrintNode(root -> left);
        PrintNode(root -> right);
    }
    bool InsertNode(Node *&root, int key){
        if(!root){
            root = new Node(key);
            return true;
        }
        if(root -> value > key){
            return InsertNode(root->left, key);
        }
        else if(root -> value < key){
            return InsertNode(root->right, key);
        }
        else{
            return false;
        }
    }
    bool ContainsNode(Node *&root, int key){
        if(!root){
            return false;
        }
        if(root->value == key){
            return true;
        }
        if(root->value > key){
            return ContainsNode(root->left, key);
        }
        if(root->value > key){
            return ContainsNode(root->left, key);
        }
    }
    bool EraseNode(Node*& root, int key) {
        if (!root) return false;
        if (root->value == key) {
            if (root->left == nullptr && root->right == nullptr) {
                delete root;
                root = nullptr;
                return true;
            }
            else if (root->left != nullptr && root->right == nullptr) {
                Node* temp = root;
                root = root->left;
                delete temp;
                return true;
            }
            else if (root->left == nullptr && root->right != nullptr) {
                Node* temp = root;
                root = root->right;
                delete temp;
                return true;
            }
            else if (root->left != nullptr && root->right != nullptr) {
                Node* max_node = root->left;

                while (max_node->right) {
                    max_node = max_node->right;
                }

                root->value = max_node->value;
                return EraseNode(root->left, max_node->value);
            }
        }
        if (key < root->value) return EraseNode(root->left, key);
        if (key > root->value) return EraseNode(root->right, key);
        return false;
    }
    int IsStrictlyBalanced(Node *root){
        if (!root) return 0;
        int right_height = IsStrictlyBalanced(root->right);
        if (right_height == -1) return -1;
        int left_height = IsStrictlyBalanced(root->left);
        if (left_height == -1) return -1;
        if (abs(right_height - left_height) > 1) return -1;
        return max(right_height,left_height)+1;
    }
    public:
    Tree(): root(nullptr){}
    Tree(Node *root): root(root){}
    Tree(const Tree &other): root(CopyTree(other.root)) {}
    ~Tree(){
        DeleteTree(root);
    }
    Tree &operator=(const Tree &other){
        if (this == &other){
            return *this;
        }
        DeleteTree(root);
        root = CopyTree(other.root);
        return *this;
    }
    void print(){
        PrintNode(root);
        cout << endl;
    }
    bool insert(int key){
        bool result = InsertNode(root, key);
        /// необходима балансировка
        return result;
    }
    bool contains(int key){
        return ContainsNode(root, key);
    }
    bool erase(int key){
        /// необходима балансировка
        return EraseNode(root, key);
    }
    bool strictly_balanced(){
        return IsStrictlyBalanced(root)!=-1;
    }
};