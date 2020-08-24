#ifndef __AVL_HPP_
#define __AVL_HPP_

#include <iostream>
#include <fstream>
#include <stack>

using namespace std;

class AVL
{
private:
    class Node
    {
        Node *parent, *left, *right;
        int height;
        string element;
        int heightOfSubtree(Node *current);

    public:
        Node(const string &e);
        Node(const string &e, Node *parent);
        Node(const string &e, Node *parent, Node *left, Node *right);

        Node *getParent() const;
        Node *getLeft() const;
        Node *getRight() const;
        string getElement() const;
        int getHeight() const;

        void setLeft(Node *);
        void setRight(Node *);
        void setParent(Node *);
        void setElement(string e);

        bool isLeft() const;
        bool isRight() const;
        bool isLeaf();
        int rightChildHeight() const;
        int leftChildHeight() const;
        int updateHeight();
        bool isBalanced();
    };

private:
    int size;
    Node *root;
    void preOrderRec(std::ostream &out, Node *current);
    AVL::Node *addRec(string str, Node *current, AVL::Node *trail);
    bool containsRec(string str, Node *current);
    AVL::Node *rmvRec(string str, Node *current);
    int balance(Node *current);
    AVL::Node *singleLeftRotation(Node *current);
    AVL::Node *doubleLeftRotation(Node *current);
    AVL::Node *singleRightRotation(Node *current);
    AVL::Node *doubleRightRotation(Node *current);

public:
    class Iterator
    {
    private:
        Node *current;
        stack<Node *> st;

    public:
        Iterator();
        Iterator(const AVL &avl);
        Iterator(Node *end);
        Iterator &operator++();
        Iterator operator++(int a);
        string operator*();
        bool operator!=(Iterator it);
        bool operator==(Iterator it);
        bool hasNext();
    };

    Iterator begin() const;
    Iterator end() const;

    static const int MAX_HEIGHT_DIFF = 1;
    AVL();
    AVL(const AVL &);
    bool contains(string e);
    bool add(string e);
    bool rmv(string e);
    void print2DotFile(char *filename);
    void pre_order(std::ostream &out);

    friend std::ostream &operator<<(std::ostream &out, const AVL &tree);
    AVL &operator=(const AVL &avl);
    AVL operator+(const AVL &avl);
    AVL &operator+=(const AVL &avl);
    AVL &operator+=(const string &e);
    AVL &operator-=(const string &e);
    AVL operator+(const string &e);
    AVL operator-(const string &e);
};

#endif
