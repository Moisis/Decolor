#pragma once

#include <cstddef>
#include <iostream>

template<class T>
struct QueueNode {
  T val;
  QueueNode<T>* nxt;
public:
  QueueNode(T v = NULL, QueueNode* n = NULL) {
    val = v;
    nxt = n;
  }
  ~QueueNode() {
    delete nxt;
  }
};

template<class T>
class Queue {
public:
  Queue();
  ~Queue();
  void push(T val);
  void pop();
  unsigned int size() const;
  T peek() const;
  bool empty() const;

private:
  QueueNode<T>* head;
  unsigned int mySize;

};

template<class T>
Queue<T>::Queue() {
  head = NULL;
  mySize = 0;
}
template<class T>
Queue<T>::~Queue() {
  while(!empty()) {
    pop();
  }
}

template<class T>
void Queue<T>::push(T val) {
  if (!head) {
    head = new QueueNode<T>(val);
  } else {
    QueueNode<T>* cur = head;
    while (cur->nxt) {
      cur = cur->nxt;
    }
    cur->nxt = new QueueNode<T>(val);
  }
  mySize++;
}


template<class T>
void Queue<T>::pop() {
  if (mySize == 1) {
    head = NULL;
  } else if (mySize > 1) {
    head = head->nxt;
  }
  mySize--;
}

template<class T>
unsigned int Queue<T>::size() const {
  return mySize;
}

template<class T>
bool Queue<T>::empty() const {
  return mySize == 0;
}

template<class T>
T Queue<T>::peek() const {
  if(head) {
    return head->val;
  } else {
    T garbage;
    std::cerr << "head is null so returning garbage value\n";
    return garbage;
  }
}


