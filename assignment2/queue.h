#ifndef QUEUE_H
#define QUEUE_H

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
class Queue {
  Node<type> *_front, *_rear;
  int _size;

public:
  // Constructor
  explicit Queue();

  // Destructor
  ~Queue();

  // Store new element
  void push(const type& item);

  // Return first element
  type& front() const;

  // Remove first element
  void pop();

  // Return true if empty, false otherwise
  bool empty() const;

  // Number of elements stored
  int size() const;

};

// Implement functions below

template <typename type>
Queue<type>::Queue() {
  _front = NULL;
  _rear = NULL;
  _size = 0;
}

template <typename type>
Queue<type>::~Queue() {
  while (!empty())
    pop();
}


template <typename type>
void Queue<type>::push(const type &item) {
  if (empty()) {
    _rear = _front = new Node<type>(item);
  }
  else {
    _rear = _rear->next = new Node<type>(item);
  }
  _size++;
}

template <typename type>
type& Queue<type>::front() const {
  return _front->value;
}

template <typename type>
void Queue<type>::pop() {
  Node<type>* temp = _front;
  _front = _front->next;
  delete temp;
  _size--;
}

template <typename type>
bool Queue<type>::empty() const {
  return _size == 0;
}

template <typename type>
int Queue<type>::size() const {
  return _size;
}

#endif //QUEUE_H
