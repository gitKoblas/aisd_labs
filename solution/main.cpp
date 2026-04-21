#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Node
{
    int value;
    Node *left;
    Node *right;
    int height;
    Node(int value) : value(value), left(nullptr), right(nullptr), height(1) {}
};

class Tree
{
private:
    Node *root;

    Node *CopyTree(Node *root)
    {
        if (!root)
            return nullptr;
        Node *NewNode = new Node(root->value);
        NewNode->left = CopyTree(root->left);
        NewNode->right = CopyTree(root->right);
        NewNode->height = root->height;
        return NewNode;
    }

    void DeleteTree(Node *root)
    {
        if (!root)
            return;
        DeleteTree(root->left);
        DeleteTree(root->right);
        delete root;
    }

    void PrintNode(Node *root)
    {
        if (!root)
            return;
        PrintNode(root->left);
        cout << root->value << " ";
        PrintNode(root->right);
    }

    bool InsertNode(Node *&root, int key)
    {
        if (!root)
        {
            root = new Node(key);
            return true;
        }
        if (key < root->value)
        {
            bool result = InsertNode(root->left, key);
            root = BalanceNode(root);
            return result;
        }
        else if (key > root->value)
        {
            bool result = InsertNode(root->right, key);
            root = BalanceNode(root);
            return result;
        }
        return false;
    }

    bool ContainsNode(Node *root, int key)
    {
        if (!root)
            return false;
        if (root->value == key)
            return true;
        if (key < root->value)
            return ContainsNode(root->left, key);
        return ContainsNode(root->right, key);
    }

    bool EraseNode(Node *&root, int key)
    {
        if (!root)
            return false;
        bool result = false;
        if (key < root->value)
        {
            result = EraseNode(root->left, key);
        }
        else if (key > root->value)
        {
            result = EraseNode(root->right, key);
        }
        else
        {
            result = true;
            if (root->left == nullptr || root->right == nullptr)
            {
                Node *temp = root->left ? root->left : root->right;
                if (temp == nullptr)
                {
                    temp = root;
                    root = nullptr;
                }
                else
                {
                    *root = *temp;
                }
                delete temp;
            }
            else
            {
                Node *max_node = root->left;
                while (max_node->right)
                    max_node = max_node->right;
                root->value = max_node->value;
                EraseNode(root->left, max_node->value);
            }
        }

        if (root)
            root = BalanceNode(root);
        return result;
    }

    int NodeHeight(Node *root)
    {
        if (!root)
            return 0;
        return root->height;
    }

    void UpdateHeight(Node *root)
    {
        if (root)
        {
            root->height = 1 + max(NodeHeight(root->left), NodeHeight(root->right));
        }
    }

    Node *BalanceNode(Node *root)
    {
        if (!root)
            return nullptr;
        UpdateHeight(root);
        int balance = NodeHeight(root->left) - NodeHeight(root->right);

        if (balance > 1)
        {
            if (NodeHeight(root->left->left) < NodeHeight(root->left->right))
            {
                root->left = LeftRotation(root->left);
            }
            return RightRotation(root);
        }
        if (balance < -1)
        {
            if (NodeHeight(root->right->right) < NodeHeight(root->right->left))
            {
                root->right = RightRotation(root->right);
            }
            return LeftRotation(root);
        }
        return root;
    }

    Node *LeftRotation(Node *root)
    {
        Node *B = root->right;
        root->right = B->left;
        B->left = root;
        UpdateHeight(root);
        UpdateHeight(B);
        return B;
    }

    Node *RightRotation(Node *root)
    {
        Node *B = root->left;
        root->left = B->right;
        B->right = root;
        UpdateHeight(root);
        UpdateHeight(B);
        return B;
    }

    int IsStrictlyBalanced(Node *root)
    {
        if (!root)
            return 0;
        int left_h = IsStrictlyBalanced(root->left);
        if (left_h == -1)
            return -1;
        int right_h = IsStrictlyBalanced(root->right);
        if (right_h == -1)
            return -1;

        if (abs(left_h - right_h) > 1)
            return -1;
        return max(left_h, right_h) + 1;
    }

    void CollectValues(Node *root, vector<int> &values) const
    {
        if (!root)
            return;
        CollectValues(root->left, values);
        values.push_back(root->value);
        CollectValues(root->right, values);
    }

public:
    Tree() : root(nullptr) {}
    Tree(const Tree &other) : root(CopyTree(other.root)) {}
    ~Tree()
    {
        DeleteTree(root);
    }
    Tree &operator=(const Tree &other)
    {
        if (this == &other)
            return *this;
        DeleteTree(root);
        root = CopyTree(other.root);
        return *this;
    }
    void print()
    {
        PrintNode(root);
        cout << endl;
    }
    bool insert(int key)
    {
        return InsertNode(root, key);
    }
    bool contains(int key)
    {
        return ContainsNode(root, key);
    }
    bool erase(int key)
    {
        return EraseNode(root, key);
    }
    bool strictly_balanced()
    {
        return IsStrictlyBalanced(root) != -1;
    }
    vector<int> get_values() const
    {
        vector<int> values;
        CollectValues(root, values);
        return values;
    }
};

size_t lcg()
{
    static size_t x = 0;
    x = (1021 * x + 24631) % 116640;
    return x;
}

vector<int> intersection_values(const Tree &tree1, const Tree &tree2)
{
    vector<int> values1 = tree1.get_values();
    vector<int> values2 = tree2.get_values();
    vector<int> result;
    for (size_t i = 0; i < values1.size(); ++i)
    {
        for (size_t j = 0; j < values2.size(); ++j)
        {
            if (values1[i] == values2[j])
            {
                result.push_back(values1[i]);
                break;
            }
        }
    }
    return result;
}

vector<int> union_values(const Tree &tree1, const Tree &tree2)
{
    Tree temp = tree1;
    vector<int> v2 = tree2.get_values();
    for (int val : v2)
    {
        temp.insert(val);
    }
    return temp.get_values();
}

int main()
{
    Tree tree1;
    Tree tree2;
    for (int i = 0; i < 10; i++)
        tree1.insert(lcg() % 100);
    for (int i = 0; i < 10; i++)
        tree2.insert(lcg() % 100);
    tree1.insert(52);
    tree2.insert(52);

    cout << "Tree 1: ";
    tree1.print();
    cout << "Tree 2: ";
    tree2.print();

    vector<int> inter = intersection_values(tree1, tree2);
    cout << "Intersection: ";
    for (int v : inter)
        cout << v << " ";
    cout << endl;

    return 0;
}