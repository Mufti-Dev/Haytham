// BST_Impl.cpp : This file contains the 'main' function. Program execution begins and ends there.
// This is binary tree implementation using template

#include <iostream>
#include <algorithm>

template <typename T>
struct Node {
public:
    Node() :_root(0), left(0), right(0) {
        _root = 0;
        left = 0;
        right = 0;
    }

    void InsertNode(T Data) {
        _root = Insert(_root, Data);
    }

    Node<T>* FindNodeByData(T data) {
        return FindNode(_root, data);
    }

    bool IsNodeData(T Data) {
        return dataFound(_root, Data);
    }

    Node<T>* FindMinNode() {
        if (!_root) return nullptr;

        auto* tempPtrNode = _root;

        while (nullptr != tempPtrNode->left) {
            tempPtrNode = tempPtrNode->left;
        }
        return tempPtrNode;
    }

    Node<T>* FindMaxNode() {
        if (!_root) return nullptr;

        auto* tempPtrNode = _root;

        while (nullptr != tempPtrNode->right) {
            tempPtrNode = tempPtrNode->right;
        }
        return tempPtrNode;
    }

    size_t getHeight() {
        return getHeight(_root);
    }

    void printData(Node<T>* nodePtr) {
        if (nodePtr) {
            std::cout << nodePtr->data << std::endl;
        }
    }

    bool isBalanced() {
        return isBalanced(_root);
    }

    Node<T>* getNextNode(Node<T>* anyNode) {
        return getNextNode(_root, anyNode);
    }

    Node<T>* DeleteNode(T Data)
    {
        return DeleteNode(_root, Data);
    }

    void deleteTree() {
        deleteTree(_root);
    }

private:
    T data;
    Node<T>* left, *right, *_root;

private:
    Node<T>* Insert(Node<T>* root, int data) {
        if (0 == root) {
            root = GetNewNode(data);
        }
        else if (data <= root->data) {
            root->left = Insert(root->left, data);
        }
        else {
            root->right = Insert(root->right, data);
        }
        return root;
    }

    Node<T>* GetNewNode(T data)
    {
        Node<T>* newNode = new Node<T>();
        newNode->data = data;
        newNode->left = newNode->right = NULL;
        return newNode;
    }

    size_t getHeight(Node<T> *pNode) {

        if (!pNode) return 0;
        return 1 + std::max(getHeight(pNode->left), getHeight(pNode->right));
    }

    bool isBalanced(Node<T>* pRoot) {
        if (!pRoot) return false;

        int nLeftHeight = getHeight(pRoot->left);
        int nRightHeight = getHeight(pRoot->right);

        if (std::abs(nLeftHeight - nRightHeight) > 1)
            return false;
        return true;
    }

    Node<T>* FindNode(Node<T>* root, T data) {
        if (0 == root) return root;
        else if (data == root->data) return root;
        else if (data <= root->data) return FindNode(root->left, data);
        else return FindNode(root->right, data);
    }

    bool dataFound(Node<T>* root, T data) {
        if (0 == root) return false;
        else if (data == root->data) return true;
        else if (data <= root->data) return dataFound(root->left, data);
        else return dataFound(root->right, data);
    }

    Node<T>* DeleteNode(Node<T>* pNode, T data) {
        if (!pNode) return pNode;
        else if (data < pNode->data) pNode->left = DeleteNode(pNode->left, data);
        else if (data > pNode->data) pNode->right = DeleteNode(pNode->right, data);
        else
        {
            // Found and get ready to be deleted
            if (NULL == pNode->left && NULL == pNode->right) { /*Leaf node*/
                delete pNode;
                pNode = NULL;
            }
            else if (NULL == pNode->left) {/*One child node*/
                auto* pTemp = pNode;
                pNode = pNode->right;
                delete pTemp;
            }
            else if (NULL == pNode->right) {
                auto* pTemp = pNode;
                pNode = pNode->left;
                delete pTemp;
            }
            else {
                auto* pTemp = FindMinOfRight(pNode->right);
                pNode->data = pTemp->data;
                pNode->right = DeleteNode(pNode->right, pTemp->data);
            }
        }

        return pNode;
    }

    Node<T>* FindMinOfRight(Node<T>* root) {
        while (nullptr != root->left) {
            root = root->left;
        }
        return root;
    }

    Node<T>* getNextNode(Node<T>* root, Node<T>* anyNode) { /*Search for inorder successor for anyNode*/

        if (NULL != anyNode->right) return FindMinOfRight(anyNode->right);

        Node<T>* nextNode = NULL;

        while (root != anyNode)
        {
            if (anyNode->data < root->data) {
                nextNode = root;
                root = root->left;
            }
            else {
                root = root->right;
            }
        }

        return nextNode;
    }

    void deleteTree(Node<T>* treeNode) {
        if (!treeNode) return;

        auto* currNode = treeNode;
        auto* leftNode = treeNode->left;
        auto* rightNode = treeNode->right;

        delete(currNode);
        deleteTree(leftNode);
        deleteTree(rightNode);
    }
};

int main()
{
    Node<int> nd;
    nd.InsertNode(15);
    nd.InsertNode(10);
    nd.InsertNode(20);
    nd.InsertNode(25);
    nd.InsertNode(8);
    nd.InsertNode(6);
    nd.InsertNode(12);
    nd.InsertNode(26);

    int number = 8;
    if (nd.IsNodeData(number)) {
        std::cout << "Found..." << std::endl;
    }
    else {
        std::cout << "Not Found..." << std::endl;
    }

    auto* pNodeMin = nd.FindMinNode();
    nd.printData(pNodeMin);

    auto* pNodeMax = nd.FindMaxNode();
    nd.printData(pNodeMax);

    auto* pNode = nd.FindNodeByData(6);

    auto* pNodeNext = nd.getNextNode(pNode);
    nd.printData(pNodeNext);

    auto nHeight = nd.getHeight();
    auto bBalanced = nd.isBalanced();

    nd.DeleteNode(10);

    // Delete complete tree
    nd.deleteTree();

    return 0;
}