#ifndef TO_POSTFIX_H
#define TO_POSTFIX_H

#include <iostream>
#include <sstream>
#include "stack.h"

using namespace std;

bool isdecimal(char c) {
    return '0' <= c && c <= '9';
}

int get_priority(char c) {
    switch (c) {
    case '(':
        return 3;
    case '*':
    case '/':
        return 2;
    case '+':
    case '-':
        return 1;
    default:
        return -1;
    }
}

string to_postfix(const string& infix) {
    stringstream temp, in, out;
    string result, tmp;
    double num;
    char c;
    bool flag = true;   // if before was operator
    bool isnum;         // if this is number
    Stack<char> stack;

    /* deleting spaces */
    temp.str(infix);
    while (temp >> tmp)
        in << tmp;
    
    while (in >> c) {
        isnum = false;
        if (flag && c == '-') {
            char t;
            in >> t;
            if (isdecimal(t)) {
                in.putback(t);
                in.putback(c);
                in >> num;
                isnum = true;
                flag = false;
            }
            else {
                in.putback('(');
                in.putback('*');
                num = -1;
                isnum = true;
                flag = false;
            }
        }
        else if (isdecimal(c)) {
            in.putback(c);
            in >> num;
            isnum = true;
            flag = false;
        }
        else if (c == ')') {
            flag = false;
        }
        else {
            flag = true;
        }

        if (isnum) {
            out << num << ' ';
        }
        else {
            if (stack.empty()) {
                stack.push(c);
            }
            else {
                if (c == ')') {
                    for (; stack.top() != '('; stack.pop())
                        out << stack.top() << ' ';
                    stack.pop();
                }
                else {
                    for (; !stack.empty() && get_priority(stack.top()) >= get_priority(c) && stack.top() != '('; stack.pop())
                        out << stack.top() << ' ';
                    stack.push(c);
                }
            }
        }
    }

    for (; !stack.empty(); stack.pop())
        out << stack.top() << ' ';

    getline(out, result);

    return result;
}

#endif //TO_POSTFIX_H
