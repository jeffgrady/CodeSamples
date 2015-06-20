// Author:  Jeff Grady <jeff.grady@gmail.com>
// Date:    3/17/2013
// 
// Description:  This file provides an interface and implementation for
//               a doubly-linked list utilizing Node.h
//
// NOTE:  It is worth mentioning that I would almost never consider
//        writing my own linked list implementation given the amount
//        of available code in standard libraries, Boost, etc.
// 
//        The purpose of this code is to demonstrate I have basic
//        proficiency in C++ and reasonably good coding practices.
//
// Copyright (c) 2013 Jeffrey T. Grady, All Rights Reserved
//

#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <cstddef>
#include "Node.h"

template <class T>
class List {
 public:
  // Make default and copy constructors explicit to avoid any implicit
  // type conversion.
  explicit List();
  explicit List(const List &list);

  // Make destructor virtual in case anyone ever subclasses
  virtual ~List();

  void clear();
  void insertAtFront(const T &data);
  void insertAtEnd(const T &data);
  void deleteAtFront();
  void deleteAtEnd();
  Node<T> * findNode(const T &find_data) const;
  void deleteNode(Node<T> *node);
  bool isNotFound(const Node<T> *node) const;
  std::size_t getLength() const;

  List & operator=(const List &list);

  // I received linker errors if I didn't put this method inside the
  // class definition.
  friend std::ostream & operator<<(std::ostream &o, List<T> &list)
    {
      Node<T> *tmp;
      o << "<list> ";
      for (tmp = list.head->getNext();
	   tmp != list.tail;
	   tmp = tmp->getNext()) {
	o << tmp->getData() << " ";
      }
      o << "</list> ";
      return o;
    }

 protected:
  // 'head' represents the start of the list, 'tail' the end
  Node<T> *head;
  Node<T> *tail;
  // 'length' contains the number of nodes in the list
  std::size_t length;

 private:
  void init();
  
};

// init() is a private method that takes no arguments.  It initializes
// 'head' and 'tail' and sets their pointers appropriately.  Returns nothing.
template <class T>
void List<T>::init()
{
  head = new Node<T>;
  tail = new Node<T>;
  head->setNext(tail);
  tail->setPrev(head);
}

// Default constructor.  Takes no arguments, returns nothing.  Only calls
// init() to perform initialization.
template <class T>
List<T>::List() :
  head(),
  tail(),
  length(0)
    
{
  init();
}

// Copy constructor.  Takes a const List reference as an argument.
// Calls init() and then invokes operator=() to copy the data.
// Returns nothing.
template <class T>
List<T>::List(const List &list) :
  head(),
  tail(),
  length(0)
{
  init();
  *this = list;
}

// Destructor.  Clears what List has allocated for itself.
// All references that were stored in the List are now invalid.
template <class T>
List<T>::~List()
{
  clear();
  delete head;
  delete tail;
}

// clear() erases the contents of the list.  It takes no arguments and
// returns nothing.
template <class T>
void List<T>::clear()
{
  Node<T> *first = head->getNext();
  Node<T> *second = first->getNext();
  while (first != tail) {
    delete first;
    first = second;
    second = second->getNext();
  }
  head->setNext(tail);
  tail->setPrev(head);
  length = 0;
}

// insertAtFront() takes a const reference T as an argument, creates
// a new node at the front of the list and sets its data equal to what is 
// stored in 'data'.  Returns nothing.
template <class T>
void List<T>::insertAtFront(const T &data)
{
  Node<T> *new_node = new Node<T>;
  new_node->setData(data);
  new_node->setNext(head->getNext());
  new_node->setPrev(head);
  new_node->getNext()->setPrev(new_node);
  head->setNext(new_node);
  length += 1;
}

// insertAtEnd() takes a const reference T as an argument, creates
// a new node at the end of the list and sets its data equal to what is 
// stored in 'data'.  Returns nothing.
template <class T>
void List<T>::insertAtEnd(const T &data)
{
  Node<T> *new_node = new Node<T>;
  new_node->setData(data);
  new_node->setNext(tail);
  new_node->setPrev(tail->getPrev());
  tail->setPrev(new_node);
  tail->getPrev()->getPrev()->setNext(new_node);
  length += 1;
}

// deleteAtFront() takes no arguments and deletes the node at the front
// of the list.  Returns nothing.
template <class T>
void List<T>::deleteAtFront()
{
  // empty list
  if (head->getNext() == tail) return;
  Node<T> *tmp;
  tmp = head->getNext();
  head->getNext()->getNext()->setPrev(head);
  head->setNext(head->getNext()->getNext());
  delete tmp;
  length -= 1;
}

// deleteAtEnd() takes no arguments and deletes the node at the end
// of the list.  Returns nothing.
template <class T>
void List<T>::deleteAtEnd()
{
  // empty list
  if (head->getNext() == tail) return;
  Node<T> *tmp;
  tmp = tail->getPrev();
  tail->getPrev()->getPrev()->setNext(tail);
  tail->setPrev(tail->getPrev()->getPrev());
  delete tmp;
  length -= 1;
}

// deleteNode() takes a Node<T> * as an argument and removes that node
// from the list.  This node was most likely found with findNode().
// Returns nothing.
template <class T>
void List<T>::deleteNode(Node<T> *node)
{
  // Make sure we don't do something bad by accident...
  if ( (node == (Node<T> *)0) || (node == head) || (node == tail) ) return;
  node->getPrev()->setNext(node->getNext());
  node->getNext()->setPrev(node->getPrev());
  delete node;
  length -= 1;
}

// findNode() takes a const reference T as an argument, finds the first
// instance of the data specified by 'find_data' in the list, and returns
// that Node<T> * const.  If the data is not found, it returns a
// pointer to the end of the list.  Also, it makes the assumption that
// operator==() is adequate to find the data you're looking for.
template <class T>
Node<T> * List<T>::findNode(const T &find_data) const
{
  for (Node<T> *tmp = head->getNext();
       tmp != tail;
       tmp = tmp->getNext()) {
    if (tmp->getData() == find_data)
      return tmp;
  }
  return tail;
}

// isNotFound() takes a Node<T> * as an argument and compares that pointer to
// the end of the list.  If they are the same, it returns true, if not, 
// false.
template <class T>
bool List<T>::isNotFound(const Node<T> *node) const
{
  if (node == tail)
    return true;
  return false;
}

// getLength() takes no arguments and returns the std::size_t length of 
// the list.
template <class T>
std::size_t List<T>::getLength() const
{
  return length;
}

// operator=() takes a const reference to List<T> as an argument and 
// creates a deep copy of the list at the right-hand side of the assignment.
// Returns a reference to List<T>.
template <class T>
List<T> & List<T>::operator=(const List<T> &list)
{
  this->clear();
  for (Node<T> *tmp = list.head->getNext();
       tmp != list.tail;
       tmp = tmp->getNext()) {
    this->insertAtEnd(tmp->getData());
  }
  return *this;
}

#endif
