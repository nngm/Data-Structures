#ifndef STACK_H
#define STACK_H

#include <iostream>

using namespace std;

#ifndef CLASS_NODE
#define CLASS_NODE
template <typename T>
class Node {
 public:
  T value;
  Node<T>* next;

  Node(T v): value(v), next(NULL) {}
  Node(T v, Node<T>* n): value(v), next(n) {}
};
#endif

template <typename type>
class Stack {
  Node<type>* _top;
  int _size;

public:
  // Constructor
  explicit Stack();

  // Destructor
  ~Stack();

  // Store new element
  void push(const type& item);

  // Return first element
  type& top() const;

  // Remove first element
  void pop();

  // Return true if empty, false otherwise
  bool empty() const;

  // Number of elements stored
  int size() const;
};

// Implement functions below

template <typename type>
Stack<type>::Stack() {
  _top = NULL;
  _size = 0;
}

template <typename type>
Stack<type>::~Stack() {
  while (!empty())
    pop();
}

template <typename type>
void Stack<type>::push(const type &item) {
  _top = new Node<type>(item, _top);
  _size++;
}

template <typename type>
type& Stack<type>::top() const {
  return _top->value;
}

template <typename type>
void Stack<type>::pop() {
  Node<type>* temp = _top;
  _top = _top->next;
  delete temp;
  _size--;
}

template <typename type>
bool Stack<type>::empty() const {
  return _size == 0;
}

template <typename type>
int Stack<type>::size() const {
  return _size;
}

#endif //STACK_H
