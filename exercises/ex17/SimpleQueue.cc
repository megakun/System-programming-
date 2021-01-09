/*
 * Copyright ©2020 Travis McGaha.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Summer Quarter 2020 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include "SimpleQueue.h"

SimpleQueue::SimpleQueue() {
  size = 0;
  front = nullptr;
  end = nullptr;
  pthread_mutex_init(&q_lock, nullptr);
}

SimpleQueue::~SimpleQueue() {
  while (front != nullptr) {
    node *next = front->next;
    delete front;
    front = next;
  }
  pthread_mutex_destroy(&q_lock);
}

void SimpleQueue::Enqueue(string item) {
  node *new_node = new node();
  new_node->next = nullptr;
  new_node->item = item;
  if (end != nullptr) {
    end->next = new_node;
  } else  {
    front = new_node;
  }
  end = new_node;
  size++;
  pthread_mutex_unlock(&q_lock);
}

bool SimpleQueue::Dequeue(string *result) {
  if (size == 0) {
    return false;
  }
  *result = front->item;
  node *next = front->next;
  delete front;
  if (end == front) {
    end = front = next;
  } else {
    front = next;
  }
  size--;
  return true;
}

int SimpleQueue::Size() const {
  return size;
}

bool SimpleQueue::IsEmpty() const {
  return size == 0;
}
