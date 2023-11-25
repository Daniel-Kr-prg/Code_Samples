#include <functional>
#include "bst_tree.h"
#include "iostream"

//void bst_tree::insert(long long data) {
//
//    node * new_node = new node(data);
//    std::atomic<node*> * goal = &root;
//    node* resultNode;
//    while (true)
//    {
//        resultNode = (*goal).load();
//        if ((resultNode == nullptr && (*goal).compare_exchange_weak(resultNode, new_node)) || resultNode->data == data)
//            return;
//        else
//        {
//            if (data < resultNode->data)
//            {
//                goal = &(resultNode->left);
//            }
//            else
//            {
//                goal = &(resultNode->right);
//            }
//        }
//    }
//}

void bst_tree::insert(long long data) 
{
    node* new_node = new node(data);
    std::atomic<node*>* goal = &root;
    node* resultNode = (*goal).load();
    while ((resultNode != nullptr) || !(*goal).compare_exchange_weak(resultNode, new_node))
    {
        if (resultNode->data == data) 
        {
            return;
        }
        else if (data > resultNode->data) 
        {
            goal = &(resultNode->right);
        }
        else 
        {
            goal = &(resultNode->left);
        }
        resultNode = (*goal).load();
    }
}

bst_tree::~bst_tree() {
    // Rekurzivni funkce pro pruchod stromu a dealokaci pameti prirazene jednotlivym
    // uzlum
    std::function<void(node*)> cleanup = [&](node * n) {
        if(n != nullptr) {
            cleanup(n->left);
            cleanup(n->right);

            delete n;
        }
    };
    cleanup(root);
}
