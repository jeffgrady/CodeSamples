// Author:  Jeff Grady <jeff.grady@gmail.com>
// Date:    3/17/2013
// 
// Description:  This file provides an interface and implementation for
//               a Node of a doubly-linked list.
//
// Copyright (c) 2013 Jeffrey T. Grady, All Rights Reserved
//

#ifndef NODE_H
#define NODE_H

template <class T>
class Node {
 protected:
  // 'next' is a pointer to the next node in the list, 'prev' is a pointer
  // to the previous, as you might expect, and 'data' stores whatever the
  // user has specified.
  Node *next;
  Node *prev;
  T data;

 public:
  // Make default and copy constructors explicit to avoid any implicit
  // type conversion.
  explicit Node();
  explicit Node(const Node &node);

  // Make destructor virtual in case anyone ever subclasses
  virtual ~Node();
  
  // Make the pointer const, but not what it points to
  void setNext(Node * const node);
  Node * const getNext() const;

  // Make the pointer const, but not what it points to
  void setPrev(Node * const node);
  Node * const getPrev() const;

  void setData(const T &new_data);
  // Return a const reference here.  We don't want anyone doing something
  // weird with our data...
  const T & getData() const;

  Node & operator=(const Node &node);

};

// Default constructor.  Takes no arguments, returns nothing.
template <class T>
Node<T>::Node() :
  next(0),
  prev(0),
  data()
{
}

// Copy constructor.  Takes a const Node reference as an argument,
//  returns nothing.
template <class T>
Node<T>::Node(const Node &node) :
  next(node.next),
  prev(node.prev),
  data(node.data)
{
}

// Destructor.  Empty.
template <class T>
Node<T>::~Node()
{
}

// setNext() takes a Node pointer as an argument and sets 'next' equal to
// it.  Returns nothing.
template <class T>
void Node<T>::setNext(Node<T> * const node)
{
  next = node;
}

// getNext() takes no arguments and returns a Node pointer to 'next'.
template <class T>
Node<T> * const Node<T>::getNext() const
{
  return next;
}

// setPrev() takes a Node pointer as an argument and sets 'prev' equal to
// it.  Returns nothing.
template <class T>
void Node<T>::setPrev(Node<T> * const node)
{
  prev = node;
}

// getPrev() takes no arguments and returns the Node pointer to 'prev'.
template <class T>
Node<T> * const Node<T>::getPrev() const
{
  return prev;
}

// setData() takes a const reference to <class T> as an argument and assigns
// that to 'data'.  Returns nothing.
template <class T>
void Node<T>::setData(const T &new_data)
{
  data = new_data;
}

// getData() takes no arguments and returns a const reference to 'data'
// which is of type <class T>.
template <class T>
const T & Node<T>::getData() const
{
  return data;
}

// operator=() takes a const Node reference as an argument (the right hand
// side of the assignment), assigns the data members, and returns a
// Node reference to *this.
template <class T>
Node<T> & Node<T>::operator=(const Node<T> &node)
{
  next = node.next;
  prev = node.prev;
  data = node.data;
  return *this;
}

#endif
