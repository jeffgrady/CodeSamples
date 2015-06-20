// Author:  Jeff Grady <jeff.grady@gmail.com>
// Date:    3/17/2013
// 
// Description:  This is a driver program that tests the basic functionality
//               of List.h.  It uses most all the methods available and
//               uses assert() to test if our tests succeed.
//
// TODO:         Create iterator and const_iterator instead of pointers.
//
// Copyright (c) 2013 Jeffrey T. Grady, All Rights Reserved
//

#include <string>
#include <iostream>
#include <assert.h>
#include "List.h"

// Test that the clear() method leaves us with 0 elements
void test_clear()
{
  List<std::string> test;
  test.insertAtFront("a");
  test.insertAtFront("b");
  test.insertAtFront("c");
  test.insertAtFront("d");
  test.insertAtFront("e");
  test.clear();
  assert(test.getLength() == 0);
}

// Test that operator=() gives us the same elements and length.
void test_assignment()
{
  List<std::string> test;
  test.insertAtFront("a");
  test.insertAtFront("b");
  test.insertAtFront("c");
  List<std::string> list_copy;
  list_copy = test;
  assert(list_copy.getLength() == 3);
  Node<std::string> *tmp;
  tmp = list_copy.findNode("a");
  assert(tmp->getData() == "a");
  tmp = list_copy.findNode("b");
  assert(tmp->getData() == "b");
  tmp = list_copy.findNode("c");
  assert(tmp->getData() == "c");
}

// Test that the copy constructor gives us the same elements and length.
void test_copy_constructor()
{
  List<std::string> test;
  test.insertAtFront("a");
  test.insertAtFront("b");
  test.insertAtFront("c");
  List<std::string> another_copy(test);
  assert(another_copy.getLength() == 3);
  Node<std::string> *tmp;
  tmp = test.findNode("a");
  assert(tmp->getData() == "a");
  tmp = test.findNode("b");
  assert(tmp->getData() == "b");
  tmp = test.findNode("c");
  assert(tmp->getData() == "c");
}

// Test that insertAtFront() and deleteAtFront() work as expected.
void test_insertAtFront_and_deleteAtFront()
{
  List<int> test;
  Node<int> *tmp;
  
  test.insertAtFront(1);
  tmp = test.findNode(1);
  assert(tmp->getData() == 1);
  test.insertAtFront(2);
  tmp = test.findNode(2);
  assert(tmp->getData() == 2);
  test.insertAtFront(3);
  tmp = test.findNode(3);
  assert(tmp->getData() == 3);
  test.deleteAtFront();
  assert(test.getLength() == 2);
  test.deleteAtFront();
  assert(test.getLength() == 1);
  test.deleteAtFront();
  assert(test.getLength() == 0);
  test.deleteAtFront();
  assert(test.getLength() == 0);
}

// Test that insertAtEnd() and deleteAtEnd() work as expected.
void test_insertAtEnd_and_deleteAtEnd()
{
  List<int> test;
  Node<int> *tmp;
  
  test.insertAtEnd(1);
  tmp = test.findNode(1);
  assert(tmp->getData() == 1);
  test.insertAtEnd(2);
  tmp = test.findNode(2);
  assert(tmp->getData() == 2);
  test.insertAtEnd(3);
  tmp = test.findNode(3);
  assert(tmp->getData() == 3);
  test.deleteAtEnd();
  assert(test.getLength() == 2);
  test.deleteAtEnd();
  assert(test.getLength() == 1);
  test.deleteAtEnd();
  assert(test.getLength() == 0);
  test.deleteAtEnd();
  assert(test.getLength() == 0);
}

// Test that findNode(), isNotFound(), and deleteNode() work as expected.
void test_findNode()
{
  List<std::string> test;
  test.insertAtFront("a");
  test.insertAtFront("b");
  test.insertAtFront("c");

  Node<std::string> *node;
  node = test.findNode("b");
  assert(node->getData() == "b");

  if (! test.isNotFound(node)) {
    test.deleteNode(node);
  }
  assert(test.getLength() == 2);

  node = test.findNode("c");
  assert(node->getData() == "c");

  if (! test.isNotFound(node)) {
    test.deleteNode(node);
  }
  assert(test.getLength() == 1);

  node = test.findNode("z");

  assert(test.isNotFound(node));
  assert(test.getLength() == 1);
}

// Test that operator<<() works.
void test_stream_operator()
{
  List<std::string> test;
  test.insertAtEnd("foo");
  test.insertAtEnd("bar");
  test.insertAtEnd("baz");
  std::cout << test << std::endl;
}

// Test that "large" lists work.
void test_big_list() {
  List<int> test;
  int size = 1000000;
  for (int i = 0; i < size; i++) {
    test.insertAtFront(i);
  }
  assert(test.getLength() == size);
  test.clear();
  assert(test.getLength() == 0);
}

int main(int argc, char *argv[])
{
  test_clear();
  test_assignment();
  test_copy_constructor();
  test_insertAtFront_and_deleteAtFront();
  test_insertAtEnd_and_deleteAtEnd();
  test_findNode();
  test_stream_operator();
  test_big_list();

  return 0;
}
