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
    Node *CopyTree(Node *node){
        if (!node){
            return nullptr;
        }
        Node* NewNode = new Node (node -> value);
        root -> left = CopyTree(node -> left);
        root -> right = CopyTree(node -> right);
        return NewNode;

    }
    Node *DeleteTree(Node *node){
        if (!node){
            return;
        }
        DeleteTree(node -> right);
        DeleteTree(node -> left);
        delete node;
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
};