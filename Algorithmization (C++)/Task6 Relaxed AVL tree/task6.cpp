
#include <iostream>
#include <queue>

using namespace std;


class Node {
public:
    int key;
    Node* left;
    Node* right;
    int depth;

    Node(int newKey) :
        key(newKey),
        left(0),
        right(0),
        depth(0)
    {
    }
};

class Tree {
public:
    int D, totalRotations;
    Node* root;

    Tree(int d) :
        D(d),
        totalRotations(0),
        root(0)
    {
    }
    void InsertKey(int key)
    {
        root = Insert(root, key);
    }
    void DeleteKey(int key)
    {
        root = Delete(root, key);
    }
    int GetTreeDepth()
    {
        if (!root)
            return 0;
        return root->depth;
    }
private:
    Node* Insert(Node* root, int key) 
    {
        if (!root)
            return new Node(key);

        if (key < root->key)
            root->left = Insert(root->left, key);
        else if (key > root->key)
            root->right = Insert(root->right, key);
        else
            return root;

        root->depth = max(GetDepth(root->left), GetDepth(root->right)) + 1;
        
        int balance = GetBalance(root);
        if (balance < -D)
        {
            totalRotations += 1;
            if (key > root->right->key)
                return LeftRotation(root);
            else
            {
                root->right = RightRotation(root->right);
                return LeftRotation(root);
            }
        }
        else if (balance > D)
        {
            totalRotations += 1;
            if (key < root->left->key)
                return RightRotation(root);
            else
            {
                root->left = LeftRotation(root->left);
                return RightRotation(root);
            }
        }
        else 
            return root;
    }

    Node* Delete(Node* root, int key) 
    {
        if (!root)
            return root;

        if (key < root->key)
            root->left = Delete(root->left, key);
        else if (key > root->key)
            root->right = Delete(root->right, key);
        else
        {
            if (!root->left && !root->right)
            {
                delete root;
                root = 0;
            }
            else if (!root->left || !root->right)
            {
                Node* nodeToDelete;
                if (root->left)
                    nodeToDelete = root->left;
                else
                    nodeToDelete = root->right;
                *root = *nodeToDelete;
                delete nodeToDelete;
            }
            else 
            {
                Node* nodeToDelete = root->left;
                while (nodeToDelete->right)
                    nodeToDelete = nodeToDelete->right;

                root->key = nodeToDelete->key;
                root->left = Delete(root->left, nodeToDelete->key);
            }
        }

        if (root)
        {
            root->depth = max(GetDepth(root->left), GetDepth(root->right)) + 1;
            int balance = GetBalance(root);
            if (balance < -D)
            {
                totalRotations += 1;
                if (GetBalance(root->right) > 0)
                {
                    root->right = RightRotation(root->right);
                    return LeftRotation(root);
                }
                else
                    return LeftRotation(root);
                
            }
            else if (balance > D)
            {
                totalRotations += 1;
                if (GetBalance(root->left) < 0)
                { 
                    root->left = LeftRotation(root->left);
                    return RightRotation(root);
                }
                else
                    return RightRotation(root);
            } 
        }
        return root;
    }

    Node* LeftRotation(Node* root) 
    {
        Node* newRoot = root->right;
        Node* leftSubtree = newRoot->left;
        newRoot->left = root;
        root->right = leftSubtree;

        root->depth = max(GetDepth(root->left), GetDepth(root->right)) + 1;
        newRoot->depth = max(GetDepth(newRoot->left), GetDepth(newRoot->right)) + 1;

        return newRoot;
    }

    Node* RightRotation(Node* root) 
    {
        Node* newRoot = root->left;
        Node* rightSubtree = newRoot->right;
        newRoot->right = root;
        root->left = rightSubtree;

        root->depth = max(GetDepth(root->left), GetDepth(root->right)) + 1;
        newRoot->depth = max(GetDepth(newRoot->left), GetDepth(newRoot->right)) + 1;

        return newRoot;
    }

    int GetDepth(Node* node)
    {
        if (!node)
            return -1;
        return node->depth;
    }
    int GetBalance(Node* node) 
    {
        if (!node)
            return -1;
        return GetDepth(node->left) - GetDepth(node->right);
    }

    
};


int D, A, B, M, N;

int main() {

    cin >> D >> A >> B >> M >> N;

    Tree treeDefault = Tree(1);
    Tree treeRelaxed = Tree(D);

    treeDefault.InsertKey(B);
    treeRelaxed.InsertKey(B);

    int prevKey = B;

    queue<int> toDelete;

    for (int i = 2; i < N+1; i++)
    {
        int newKey = (A * prevKey + B) % M;
        treeDefault.InsertKey(newKey);
        treeRelaxed.InsertKey(newKey);
        if (i % 3 == 0)
            toDelete.push(newKey);
        prevKey = newKey;
    }
    while (!toDelete.empty())
    {
        treeDefault.DeleteKey(toDelete.front());
        treeRelaxed.DeleteKey(toDelete.front());
        toDelete.pop();
    }
    
    cout << treeDefault.GetTreeDepth() << " " << treeDefault.totalRotations << "\n";
    cout << treeRelaxed.GetTreeDepth() << " " << treeRelaxed.totalRotations << "\n";

}