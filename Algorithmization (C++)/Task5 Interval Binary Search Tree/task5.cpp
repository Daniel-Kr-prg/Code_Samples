
#include <iostream>

using namespace std;

class Node {
public: 
    Node(int Min, int Max) :
        min(Min),
        max(Max),
        left(0),
        right(0)
    {
    }

    int min;
    int max;

    Node* left;
    Node* right;

};

class iBVS
{
public:
    iBVS() : 
        root(0),
        totalNodes(0),
        totalFrees(0),
        totalConnections(0)
        {}

    Node* root;
    int totalNodes;
    int totalFrees;
    int totalConnections;


    void Insert(int minVal, int maxVal)
    {
        if (minVal > maxVal)
        {
            return;
        }
        bool uniqueNode = true;
        Node** subRoot = &root;
        while (*subRoot) {
            Node& node = **subRoot;
            if (maxVal < node.min) {
                subRoot = &node.left;
            }
            else if (minVal > node.max) {
                subRoot = &node.right;
            }
            else {
                uniqueNode = false;
                break;
            }
        }
        if (uniqueNode)
        {
            *subRoot = new Node(minVal, maxVal);
            totalNodes++;
        }
        else
        {
            if (minVal < (*subRoot)->min)
            {
                GetNewLeftSubTree(&(*subRoot)->left, &minVal);
            }
            if (maxVal > (*subRoot)->max)
            {
                GetNewRightSubTree(&(*subRoot)->right, &maxVal);
            }
            
            (*subRoot)->min = min(minVal, (*subRoot)->min);
            (*subRoot)->max = max(maxVal, (*subRoot)->max);
        }
    }

    void GetNewLeftSubTree(Node ** left, int *Min)
    {
        Node** cur = left;
        while (cur && *cur)
        {
            Node& node = **cur;
            if (*Min > node.max)
            {
                left = &(*left)->right;
                cur = left;
            }
            else 
            {
                ClearTree(&(*left)->right);

                if (!node.left || *Min > node.left->max)
                {
                    *Min = min(*Min, node.min);
                }
                Node* tmp = (*cur)->left;
                delete* cur;
                totalFrees++;
                *left = tmp;
                cur = left;
            }
        }
    }
    void GetNewRightSubTree(Node** right, int* Max)
    {
        Node** cur = right;
        while (cur && *cur)
        {
            Node& node = **cur;
            if (*Max < node.min)
            {
                right = &(*right)->left;
                cur = right;
            }
            else
            {
                ClearTree(&(*right)->left);

                if (!node.right || *Max < node.right->min)
                {
                    *Max = max(*Max, node.max);
                }
                Node* tmp = (*cur)->right;
                delete* cur;
                totalFrees++;
                *right = tmp;
                cur = right;
            }


        }
    }

    void Delete(int num)
    {
        Node** current = FindNode(num);
        if (current && *current)
        {
            bool bothChildren = true;
            Node * nodeForReplace = GetNodeForReplace(current, &bothChildren);
            if (!nodeForReplace)
            {
                delete (*current);
                totalFrees++;
                *current = 0;
            }
            else
            {
                if (bothChildren)
                {
                    nodeForReplace->left = (*current)->left;
                    nodeForReplace->right = (*current)->right;
                }
                delete (*current);
                totalFrees++;
                *current = nodeForReplace;

            }
            //totalNodes--;
        }
    }


    //Node * Del(int key, Node * root) {
    //    if (root == NULL) return root; // if the tree is empty
    //    if (key < (*root).min) // otherwise, recur down the tree
    //        (*root).left = Del(key, (*root).left);
    //    else if (key > (*root).max)
    //        (*root).right = Del(key, (*root).right);
    //    else {
    //        // node with only one child or no child
    //        if ((*root).left == NULL)
    //            return (*root).right;
    //        else if ((*root).right == NULL)
    //            return (*root).left;
    //        // node with two children
    //        (*root).key = minValue(root.right);
    //        // delete the inorder successor
    //        root.right = Del(root.key, root.right);
    //    }
    //    return root;
    //}

    Node** FindNode(int num)
    {
        Node** cur = &root;
        while (*cur)
        {
            Node& node = **cur;
            if (num < node.min)
            {
                cur = &node.left;
            }
            else if (num > node.max)
            {
                cur = &node.right;
            }
            else
            {
                return cur;
            }
        }
        return 0;
    }

    Node** FindNode(int num, Node * node)
    {
        Node** cur = &node;
        while (*cur)
        {
            Node& node = **cur;
            if (num < node.min)
            {
                cur = &node.left;
            }
            else if (num > node.max)
            {
                cur = &node.right;
            }
            else
            {
                return cur;
            }
        }
        return 0;
    }


    static Node* GetNodeForReplace(Node** root, bool* bothChildren)
    {

        if ((*root)->right)
        {
            Node** cur = &((*root)->right);
            if (!(*root)->left)
            {
                *bothChildren = false;
                return (*root)->right;
            }
            while ((*cur)->left)
            {
                Node& node = **cur;
                cur = &node.left;
            }
            Node* temp = (*cur);
            if ((*cur)->right)
            {
                *cur = (*cur)->right;
            }
            else
            {
                *cur = 0;
            }
            return temp;
        }
        else if ((*root)->left)
        {
            *bothChildren = false;
            return (*root)->left;
        }
        return 0;
    }


    static int GetTreeSum(Node* treeRoot)
    {
        if (!treeRoot)
            return 0;
        return treeRoot->max - treeRoot->min + 1 + GetTreeSum(treeRoot->left) + GetTreeSum(treeRoot->right);
    }

    static int GetTreeNodeCount(Node* treeRoot)
    {
        if (!treeRoot)
            return 0;
        return 1 + GetTreeNodeCount(treeRoot->left) + GetTreeNodeCount(treeRoot->right);
    }

    static void PrintTree(Node* treeRoot, int level = 0)
    {
        if (!treeRoot)
        {
            cout << "\n";
            return;
        }
        for (size_t i = 0; i < level; i++)
        {
            cerr << "   ";
        }
        cerr << "[" << treeRoot->min << "," << treeRoot->max << "]" << "\n";
        if (!treeRoot->left && !treeRoot->right)
            return;
        PrintTree(treeRoot->left, level + 1);
        PrintTree(treeRoot->right, level + 1);
    }

    static int GetTreeSize(Node* treeRoot, int currentSize = 0)
    {
        if (!treeRoot)
            return 0;
        if (!treeRoot->left && !treeRoot->right)
            return currentSize;
        return max(GetTreeSize(treeRoot->left, currentSize + 1), GetTreeSize(treeRoot->right, currentSize + 1));
    }

    void ClearTree(Node ** treeRoot)
    {
        if (!treeRoot || !(*treeRoot))
            return;
        ClearTree(&(*treeRoot)->left);
        ClearTree(&(*treeRoot)->right);
        delete *treeRoot;
        totalFrees++;
        *treeRoot = 0;
        //totalNodes--;
    }

    
};




int main()
{
    int N;

    cin >> N;

    iBVS bvs;

    for (size_t i = 0; i < N; i++)
    {
        char c;
        int a, b;
        cin >> c;
        switch (c)
        {
            case 'i':
                cin >> a >> b;
                bvs.Insert(a, b);
                break;
            case 'd':
                cin >> a;
                bvs.Delete(a);
            default:

                break;
        }
        //bvs.PrintTree(bvs.root);
        //cout << "NODES: " << bvs.totalNodes << " | " << "Frees: " << bvs.totalFrees << " \n";
        //cout << "NODE COUNT: " << bvs.GetTreeNodeCount(bvs.root) << " DEPTH: " << bvs.GetTreeSize(bvs.root) << " SUM" << bvs.GetTreeSum(bvs.root) << "\n";

    }
    //cerr << "NODES: " << bvs.totalNodes << " | " << "Frees: " << bvs.totalFrees << " \n";
    cout << bvs.GetTreeNodeCount(bvs.root) << " " << bvs.GetTreeSize(bvs.root) << " " << bvs.GetTreeSum(bvs.root) << "\n";
    //bvs.PrintTree(bvs.root);
    bvs.ClearTree(&bvs.root);

    //cerr << "NODES: " << bvs.totalNodes << " | " << "Frees: " << bvs.totalFrees << " \n";
}

