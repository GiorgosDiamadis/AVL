#include "AVL.hpp"
#include <iostream>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <queue>

using namespace std;
//========================================AVL implementation=====================================
AVL::AVL()
{
    root = nullptr;
    size = 0;
}

AVL::AVL(const AVL &avl)
{
    root = nullptr;
    for (AVL::Iterator it = avl.begin(); it != avl.end(); it++)
    {
        add(*it);
    }
}

bool AVL::contains(string str)
{
    return containsRec(str, root);
}

bool AVL::containsRec(string str, Node *current)
{
    if (current == nullptr)
        return false;

    string nodeElement = current->getElement();
    int compare = nodeElement.compare(str);

    if (compare == 0)
        return true;
    else if (compare < 0)
        return containsRec(str, current->getRight());
    else
        return containsRec(str, current->getLeft());
}

bool AVL::add(string str)
{
    root = addRec(str, root, nullptr);
    if (root != nullptr)
    {
        return true;
    }
    return false;
}

AVL::Node *AVL::addRec(string str, AVL::Node *current, AVL::Node *trail)
{
    if (current == nullptr)
    {
        current = new Node(str, trail);
        size++;
        return current;
    }

    string nodeElement = current->getElement();
    int compare = nodeElement.compare(str);

    if (compare < 0)
    {
        current->setRight(addRec(str, current->getRight(), current));
    }
    else if (compare > 0)
    {
        current->setLeft(addRec(str, current->getLeft(), current));
    }

    current->updateHeight();

    if (!current->isBalanced())
    {
        if (balance(current) == 2) //Tree is left heavy
        {
            if (balance(current->getLeft()) == 1)
            {
                return singleLeftRotation(current);
            }
            else if (balance(current->getLeft()) == -1)
            {
                return doubleLeftRotation(current);
            }
        }
        else if (balance(current) == -2) //Tree is right heavy
        {
            if (balance(current->getRight()) == -1)
            {
                return singleRightRotation(current);
            }
            else if (balance(current->getRight()) == 1)
            {
                return doubleRightRotation(current);
            }
        }
    }
    return current;
}

AVL::Node *AVL::singleLeftRotation(Node *current)
{
    Node *currentLeft = current->getLeft();
    Node *currentLeftRight = current->getLeft()->getRight();

    currentLeft->setRight(current);
    current->setLeft(currentLeftRight);

    current->updateHeight();
    currentLeft->updateHeight();

    if (current == root)
    {
        root = currentLeft;
    }

    return currentLeft;
}

AVL::Node *AVL::doubleLeftRotation(Node *current)
{
    Node *currentLeft = current->getLeft();
    Node *currentLeftRight = current->getLeft()->getRight();

    currentLeft->setRight(currentLeftRight->getLeft());
    current->setLeft(currentLeftRight->getRight());

    currentLeftRight->setLeft(currentLeft);
    currentLeftRight->setRight(current);

    currentLeft->updateHeight();
    current->updateHeight();
    currentLeftRight->updateHeight();

    if (current == root)
    {
        root = currentLeftRight;
    }

    return currentLeftRight;
}
AVL::Node *AVL::singleRightRotation(Node *current)
{
    Node *currentRight = current->getRight();
    Node *currentRightLeft = current->getRight()->getLeft();

    currentRight->setLeft(current);
    current->setRight(currentRightLeft);

    current->updateHeight();
    currentRight->updateHeight();

    if (current == root)
    {
        root = currentRight;
    }

    return currentRight;
}
AVL::Node *AVL::doubleRightRotation(Node *current)
{
    Node *currentRight = current->getRight();
    Node *currentRightLeft = current->getRight()->getLeft();

    currentRight->setLeft(currentRightLeft->getRight());
    current->setRight(currentRightLeft->getLeft());

    currentRightLeft->setRight(currentRight);
    currentRightLeft->setLeft(current);

    currentRight->updateHeight();
    current->updateHeight();
    currentRightLeft->updateHeight();

    if (current == root)
    {
        root = currentRightLeft;
    }

    return currentRightLeft;
}

int AVL::balance(Node *current)
{
    int leftH = current->leftChildHeight();
    int rightH = current->rightChildHeight();
    return leftH - rightH;
}

bool AVL::rmv(string str)
{
    int prevSize = size;
    root = rmvRec(str, root);

    return prevSize - size > 0;
}

AVL::Node *AVL::rmvRec(string str, Node *current)
{
    if (current == nullptr)
    {
        size--;
        return current;
    }
    string nodeElement = current->getElement();

    int compare = nodeElement.compare(str);

    if (compare < 0)
    {
        current->setRight(rmvRec(str, current->getRight()));
    }
    else if (compare > 0)
    {
        current->setLeft(rmvRec(str, current->getLeft()));
    }
    else
    {
        size--;
        //If it has no children
        if (current->isLeaf())
        {
            delete current;
            return nullptr;
        }
        else if (current->getRight() == nullptr)
        {
            Node *left = current->getLeft();
            delete current;
            return left;
        }
        else if (current->getLeft() == nullptr)
        {
            Node *right = current->getRight();
            delete current;
            return right;
        }
        else
        {
            Node *rightSubtree = current->getRight();
            Node *leftmost = rightSubtree;

            //Find left-most of right subtree
            while (rightSubtree != nullptr)
            {
                leftmost = rightSubtree;
                rightSubtree = rightSubtree->getLeft();
            }

            //Store left-most string and remove
            string leftMostElement = leftmost->getElement();
            current->setElement(leftMostElement);

            current->setRight(rmvRec(leftMostElement, current->getRight()));
        }
    }

    current->updateHeight();

    if (!current->isBalanced())
    {
        if (balance(current) == 2) //Tree is left heavy
        {
            if (balance(current->getLeft()) == 1)
            {
                return singleLeftRotation(current);
            }
            else if (balance(current->getLeft()) == -1)
            {
                return doubleLeftRotation(current);
            }
            else if (balance(current->getLeft()) == 0)
            {
                return singleLeftRotation(current);
            }
        }
        else if (balance(current) == -2) //Tree is right heavy
        {
            if (balance(current->getRight()) == -1)
            {
                return singleRightRotation(current);
            }
            else if (balance(current->getRight()) == 1)
            {
                return doubleRightRotation(current);
            }
            else if (balance(current->getRight()) == 0)
            {
                return singleRightRotation(current);
            }
        }
    }

    return current;
}

void AVL::print2DotFile(char *filename)
{
    std::ofstream file(filename);
    queue<Node *> q;
    file << "graph Tree {" << endl;

    q.push(root);
    while (!q.empty())
    {
        Node *dequeued = q.front();
        q.pop();

        uintptr_t parentId = reinterpret_cast<uintptr_t>(dequeued);
        file << parentId << " "
             << "[label="
             << "\"" << dequeued->getElement() << "\""
             << ",shape=circle,color=black]" << endl;

        Node *left = dequeued->getLeft();
        Node *right = dequeued->getRight();
        if (left != nullptr)
        {
            uintptr_t leftId = reinterpret_cast<uintptr_t>(left);

            q.push(left);
            file << parentId << " -- " << leftId << endl;
        }
        if (right != nullptr)
        {
            uintptr_t rightId = reinterpret_cast<uintptr_t>(right);

            q.push(right);
            file << parentId << " -- " << rightId << endl;
        }
    }
    file << "}";

    file.close();
}

void AVL::pre_order(std::ostream &out)
{
    preOrderRec(out, root);
}

void AVL::preOrderRec(std::ostream &out, Node *current)
{
    if (current == nullptr)
        return;
    out << current->getElement() + " ";
    preOrderRec(out, current->getLeft());
    preOrderRec(out, current->getRight());
}

std::ostream &operator<<(std::ostream &out, const AVL &tree)
{
    AVL avl;
    avl.root = tree.root;
    avl.pre_order(out);
    return out;
}

AVL &AVL::operator=(const AVL &avl)
{
    while (root != nullptr)
    {
        rmv(root->getElement());
    }

    for (AVL::Iterator it = avl.begin(); it != avl.end(); it++)
    {
        add(*it);
    }

    return *this;
}

AVL AVL::operator+(const AVL &avl)
{
    AVL newAVL;
    for (AVL::Iterator it = begin(); it != end(); it++)
    {
        newAVL.add(*it);
    }
    for (AVL::Iterator it = avl.begin(); it != avl.end(); it++)
    {
        newAVL.add(*it);
    }
    return newAVL;
}

AVL &AVL::operator+=(const AVL &avl)
{
    for (AVL::Iterator it = avl.begin(); it != avl.end(); it++)
    {
        add(*it);
    }
    return (*this);
}

AVL &AVL::operator+=(const string &e)
{
    add(e);
    return (*this);
}
AVL &AVL::operator-=(const string &e)
{
    rmv(e);
    return (*this);
}
AVL AVL::operator+(const string &e)
{
    AVL newAVL(*this);
    newAVL += e;
    return newAVL;
}
AVL AVL::operator-(const string &e)
{
    AVL newAVL(*this);
    newAVL -= e;
    return newAVL;
}
//================================Node implementation==========================================================

AVL::Node::Node(const string &e) : Node(e, nullptr, nullptr, nullptr) {}

AVL::Node::Node(const string &e, Node *parent) : Node(e, parent, nullptr, nullptr) {}

AVL::Node::Node(const string &e, Node *parent, Node *left, Node *right)
{
    height = 1;
    element = e;
    this->parent = parent;
    this->left = left;
    this->right = right;
}

AVL::Node *AVL::Node::getParent() const
{
    return this->parent;
}

AVL::Node *AVL::Node::getLeft() const
{
    return this->left;
}

AVL::Node *AVL::Node::getRight() const
{
    return this->right;
}

string AVL::Node::getElement() const
{
    return this->element;
}

void AVL::Node::setLeft(Node *node)
{
    this->left = node;
}
void AVL::Node::setRight(Node *node)
{
    this->right = node;
}
void AVL::Node::setParent(Node *node)
{
    this->parent = node;
}
void AVL::Node::setElement(string e)
{
    this->element = e;
}

bool AVL::Node::isLeft() const
{
    return this->getParent()->getLeft() == this;
}
bool AVL::Node::isRight() const
{
    return this->getParent()->getRight() == this;
}
bool AVL::Node::isBalanced()
{
    int leftH = this->rightChildHeight();
    int rightH = this->leftChildHeight();
    int balance = leftH - rightH;
    return abs(balance) < 2;
}
bool AVL::Node::isLeaf()
{
    return left == nullptr && right == nullptr;
}
int AVL::Node::rightChildHeight() const
{
    return this->getRight() == nullptr ? 0 : this->getRight()->getHeight();
}
int AVL::Node::leftChildHeight() const
{
    return this->getLeft() == nullptr ? 0 : this->getLeft()->getHeight();
}
int AVL::Node::updateHeight()
{
    this->height = heightOfSubtree(this);
    return height;
}

int AVL::Node::heightOfSubtree(Node *current)
{
    if (current == nullptr)
        return 0;
    int leftH = heightOfSubtree(current->getLeft());
    int rightH = heightOfSubtree(current->getRight());
    return max(leftH, rightH) + 1;
}

int AVL::Node::getHeight() const
{
    return this->height;
}
//=======================================================================================
//=================================Iterator implementation====================================================

AVL::Iterator::Iterator(const AVL &avl)
{
    current = avl.root;
    st.push(avl.root);
}
AVL::Iterator::Iterator(Node *end)
{
    current = end;
}
AVL::Iterator::Iterator() {}
AVL::Iterator AVL::begin() const
{

    Iterator it(*this);
    return it;
}
bool AVL::Iterator::hasNext()
{
    return !st.empty();
}

AVL::Iterator AVL::end() const
{
    Iterator it(nullptr);
    return it;
}
AVL::Iterator &AVL::Iterator::operator++()
{

    st.pop();
    if (current->getRight() != nullptr)
    {
        st.push(current->getRight());
    }
    if (current->getLeft() != nullptr)
    {
        st.push(current->getLeft());
    }
    if (!st.empty())
    {
        current = st.top();
    }
    else
    {
        current = nullptr;
    }
    return *this;
}

AVL::Iterator AVL::Iterator::operator++(int a)
{
    AVL::Iterator it = *this;
    ++(*this);
    return it;
}

string AVL::Iterator::operator*()
{
    return current->getElement();
}
bool AVL::Iterator::operator!=(Iterator it)
{
    return !(*this == it);
}
bool AVL::Iterator::operator==(Iterator it)
{
    return current == it.current;
}
//==================================================================
