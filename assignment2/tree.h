#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <sstream>
#include <cstdlib>
#include "stack.h"
#include "queue.h"

using namespace std;

/*
 * Recursive binary tree structure for building expression tree
 *
 * Constructors: Tree(char, Tree*, Tree*) and Tree(string, Tree*, Tree*)
 *      pointers will be automatically set to NULL if omitted
 *
 * Destructor: recursively destruct whole tree
 *
 */

struct Tree {
    string expr;
    Tree* left;
    Tree* right;

    explicit Tree(char h, Tree* l = NULL, Tree* r = NULL) {
        expr = string(1, h);
        left = l;
        right = r;
    }

    explicit Tree(const string& h = string(), Tree* l = NULL, Tree* r = NULL) {
        expr = h;
        left = l;
        right = r;
    }

    ~Tree() {
        delete left;
        delete right;
    }

    int size() {
        int size = 1;
        if (left != NULL) { size += left->size(); }
        if (right != NULL) { size += right->size(); }
        return size;
    }

    void print();

    double eval();
};

// -------- implement below --------

void Tree::print() {
    Queue<Tree*> queue;
    int nullstack = 0;

    queue.push(this);

    cout << '(';
    while (!queue.empty()) {
        Tree* here = queue.front();
        queue.pop();

        if (here) {
            while (nullstack --> 0)
                cout << ",null";
            nullstack++;
            
            if (this != here)
                cout << ',';
            cout << here->expr;
            queue.push(here->left);
            queue.push(here->right);
        }
        else
            nullstack++;
    }
    cout << ')' << endl;
}

double Tree::eval() {
    double num;
    stringstream ss;
    ss.str(expr);

    if (ss >> num) {
        return num;
    }
    else {
        switch (expr[0]) {
        case '+':
            return left->eval() + right->eval();
        case '-':
            return left->eval() - right->eval();
        case '*':
            return left->eval() * right->eval();
        case '/':
            return left->eval() / right->eval();
        }
    }

    return 0;
}

Tree* build_expression_tree(const string& postfix) {
    string token;
    stringstream ss;
    ss.str(postfix);

    Stack<Tree*> stack;

    while (ss >> token) {
        double num;
        stringstream temp;
        temp.str(token);
        if (temp >> num) {
            stack.push(new Tree(token));
        }
        else {
            Tree* t = new Tree(token);
            t->right = stack.top(); stack.pop();
            t->left  = stack.top(); stack.pop();
            stack.push(t);
        }
    }

    return stack.top();
}

#endif //TREE_H
