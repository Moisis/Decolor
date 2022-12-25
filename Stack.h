#pragma once


#include <iostream>

template<class T>
struct StackNode {
  T val;
  StackNode<T>* nxt;
public:
  StackNode(T v = NULL, StackNode* n = NULL) {
    val = v;
    nxt = n;
  }
  ~StackNode() {
    delete nxt;
  }
};

template<class T>
class Stack {
public:
  Stack<T>();
  ~Stack();
  void push(T);
  void pop();
  unsigned int size() const;
  T top() const;
  bool empty() const;

private:
  StackNode<T>* head;
  unsigned int mySize;

};


template<class T>
Stack<T>::Stack() {
  head = NULL;
  mySize = 0;
}

template<class T>
Stack<T>::~Stack() {
  while(!empty()) {
    pop();
  }

}

template<class T>
void Stack<T>::push(T val) {
  if (!head) {
    head = new StackNode<T>(val);
  } else {
    StackNode<T>* tmp = head;
    StackNode<T>* cur = new StackNode<T>(val);
    cur->nxt = tmp;
    head = cur;
  }
  mySize++;
}

template<class T>
void Stack<T>::pop() {
  if (mySize == 1) {
    head = NULL;
  } else if (mySize > 1) {
    head = head->nxt;
  }
  mySize--;
}

template<class T>
unsigned int Stack<T>::size() const {
  return mySize;
}

template<class T>
bool Stack<T>::empty() const {
  return mySize == 0;
}

template<class T>
T Stack<T>::top() const {
  if(head) {
    return head->val;
  } else {
    T garbage;
    std::cerr << "head is null so returning garbage\n";
    return garbage;
  }
}

