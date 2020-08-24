#include "AVL.cpp"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    AVL avl;

    avl.add("u");
    avl.add("a");
    avl.add("z");
    avl.add("b");

    avl.print2DotFile("tree.dot");

    return 0;
}
