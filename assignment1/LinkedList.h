// CSE221 Assignment 1

#ifndef LinkedList_h
#define LinkedList_h

#include <typeinfo>
#include <iostream>

template <typename T>
class Node
{
public:
	T value;
	Node<T>* next;

	Node(T value, Node<T>* next): value(value), next(next) {}
};

template <typename Type>
class LinkedList
{
	Node<Type>* head;
	int size;

public:
	// Constructor
	LinkedList();

	// Destructor
	~LinkedList();

	// Get the value located at index
	Type Get(const int index);

	// Add val at head
	void AddAtHead(const Type& val);

	// Add val at index
	void AddAtIndex(const int index, const Type& val);

	// Delete an element located at index
	void DeleteAtIndex(const int index);

	// Delete val in linked list
	void DeleteValue(const Type& val);

	// Move the first element of val to head
	void MoveToHead(const Type& val);

	// Rotate the linked list right by steps times
	void Rotate(const int steps);

	// Reduce value that repeats multiple times
	void Reduce();

	// Reverse at every k number of nodes at a time
	void K_Reverse(const int k);

	// Sort even and odd numbers separately then append
	void EvenOddSeparateSort();

	// Return the number of elements in the linked list
	int Size();

	// Delete all elements from the linked list
	void CleanUp();
	
	// Print the list
	void Print();

};

/*
 *	Implementation
 */

template <typename Type>
LinkedList<Type>::LinkedList()
{
	head = NULL;
	size = 0;
}

template <typename Type>
LinkedList<Type>::~LinkedList()
{
	CleanUp();
}

template <typename Type>
Type LinkedList<Type>::Get(const int index)
{
	if (index < 0)
		return -1;
	else if (index < size) {
		Node<Type>* cur = head;
		for (int i = index; i --> 0;)
			cur = cur->next;
		
		return cur->value;
	}
	else
		return -1;
}

template <typename Type>
void LinkedList<Type>::AddAtHead(const Type& val)
{
	head = new Node<Type>(val, head);
	size++;
}

template <typename Type>
void LinkedList<Type>::AddAtIndex(const int index, const Type& val)
{
	if (index > size)
		return;

	if (index == 0) {
		AddAtHead(val);
		return;
	}

	Node<Type>* cur = head;
	for (int i = index; i --> 1;)
		cur = cur->next;

	cur->next = new Node<Type>(val, cur->next);
	size++;
}

template <typename Type>
void LinkedList<Type>::DeleteAtIndex(const int index)
{
	if (index < 0)
		return;
		
	if (index == 0) {
		Node<Type>* temp = head->next;
		delete head;
		head = temp;
		size--;
	}
	else if (index < size) {
		Node<Type>* cur = head;
		for (int i = index; i --> 1;)
			cur = cur->next;
		
		Node<Type>* del = cur->next;

		cur->next = del->next;
		delete del;
		size--;
	}
}

template <typename Type>
void LinkedList<Type>::DeleteValue(const Type& val)
{
	if (val == head->value) {
		Node<Type>* del = head;
		head = head->next;
		delete del;
		size--;
	}
	else {
		for (Node<Type>* cur = head; cur->next != NULL; cur = cur->next) {
			Node<Type>* del = cur->next;
			if (val == del->value) {
				cur->next = del->next;
				delete del;
				size--;
				break;
			}
		}
	}
}

template <typename Type>
void LinkedList<Type>::MoveToHead(const Type& val)
{
	bool flag = false;
	for (Node<Type>* cur = head; cur != NULL; cur = cur->next) {
		if (cur->value == val) {
			flag = true;
			break;
		}
	}

	if (flag) {
		DeleteValue(val);
		AddAtHead(val);
	}
}

template <typename Type>
void LinkedList<Type>::Rotate(const int steps)
{
	for (int i = steps % size; i --> 0;) {
		AddAtHead(Get(size - 1));
		DeleteAtIndex(size - 1);
	}
}

template <typename Type>
void LinkedList<Type>::Reduce()
{
	for (Node<Type>* here = head; here != NULL; here = here->next) {
		Type value = here->value;

		for (Node<Type>* cur = here; cur != NULL && cur->next != NULL; cur = cur->next) {
			Node<Type>* del = cur->next;

			if (del->value == value) {
				cur->next = del->next;
				delete del;
				size--;
			}
		}
	}
}

template <typename Type>
void LinkedList<Type>::K_Reverse(const int k)
{
	LinkedList<Type> reversed;

	while (size) {
		LinkedList<Type> temp;

		while (size) {
			if (temp.size == k)
				break;
			
			temp.AddAtIndex(temp.size, Get(0));
			DeleteAtIndex(0);
		}

		if (temp.size == k)
			for (int i = k; i > 0; i--)
				reversed.AddAtIndex(reversed.size, temp.Get(i - 1));
		else 
			for (int i = 0; i < temp.size; i++)
				reversed.AddAtIndex(reversed.size, temp.Get(i));
	}
	
	head = reversed.head;
	size = reversed.size;
	reversed.head = NULL;
	reversed.size = 0;
}

template <typename Type>
void LinkedList<Type>::EvenOddSeparateSort() 
{
	/* selection sort */

	LinkedList<Type> sorted;

	while (size) {
		Type max = head->value;

		for (Node<Type>* cur = head; cur != NULL; cur = cur->next)
			if ([](Type a, Type b) {	// a < b
				if (a % 2) {
					if (b % 2)
						return a > b;
					else
						return false;
				}
				else {
					if (b % 2)
						return true;
					else
						return a < b;
				}
			}(max, cur->value))
				max = cur->value;
		
		DeleteValue(max);
		sorted.AddAtHead(max);
	}

	head = sorted.head;
	size = sorted.size;
	sorted.head = NULL;
	sorted.size = 0;
}

template <typename Type>
int LinkedList<Type>::Size()
{
	return size;
}

template <typename Type>
void LinkedList<Type>::CleanUp()
{
	while (size)
		DeleteAtIndex(0);
}

template <typename Type>
void LinkedList<Type>::Print()
{
	using namespace std;
	cout << '(';

	if (size > 0) {
		cout << head->value;
		for (Node<Type>* cur = head->next; cur != NULL; cur = cur->next)
			cout << ',' << cur->value;
	}

	cout << ')' << endl;
}


#endif