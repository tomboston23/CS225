#include "List.h"
/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 */

template <class T>
List<T>::List() { 
  // @TODO: graded in mp_lists part 1
    head_ = NULL;
    tail_ = NULL;
    length_ = 0;
}

/**
 * Returns a ListIterator with a position at the beginning of
 * the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::begin() const {
  // @TODO: graded in mp_lists part 1
  return List<T>::ListIterator(head_);
}

/**
 * Returns a ListIterator one past the end of the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::end() const {
  // @TODO: graded in mp_lists part 1
  return List<T>::ListIterator();
}


/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <typename T>
void List<T>::_destroy() {
  /// @todo Graded in mp_lists part 1
  ListNode * head = head_;
  ListNode * next = head;
  while(next != nullptr){
    next = head->next;
    delete head;
    head = next;
  }
  next = NULL;
  head = NULL;
}

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertFront(T const & ndata) {
  /// @todo Graded in mp_lists part 1
  ListNode * newNode = new ListNode(ndata);
  newNode -> next = head_;
  newNode -> prev = nullptr;
  
  if (head_ != nullptr) {
    head_ -> prev = newNode;
  }
  if (tail_ == nullptr) {
    tail_ = newNode;
  }
  head_ = newNode;
  length_++;
}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertBack(const T & ndata) {
  /// @todo Graded in mp_lists part 1
  ListNode * newNode = new ListNode(ndata);
  newNode->prev = tail_;
  newNode->next = NULL;
  if(tail_ != NULL){
    tail_->next = newNode;
  } 
  if(head_ == NULL){
    head_ = newNode;
  }
  tail_ = newNode;
  length_++;
}

/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List or ListNode objects!
 *
 * This function is also called by the public split() function located in
 * List-given.hpp
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk before splitting.
 * @return The starting node of the sequence that was split off.
 */
template <typename T>
typename List<T>::ListNode * List<T>::split(ListNode * start, int splitPoint) {
  /// @todo Graded in mp_lists part 1
  ListNode * curr = start;
  if(splitPoint > this->length_){
    return start;
  }
  if(start == NULL){
    return NULL;
  }
  if(splitPoint == 0){
    return start;
  }

  for (int i = 0; i < splitPoint && curr != NULL; i++) {
    curr = curr->next;
  }

  if (curr != NULL) {
      curr->prev->next = NULL;
      curr->prev = NULL;
  }

  return curr;
}

/**
 * Modifies the List using the waterfall algorithm.
 * Every other node (starting from the second one) is removed from the
 * List, but appended at the back, becoming the new tail. This continues
 * until the next thing to be removed is either the tail (**not necessarily
 * the original tail!**) or NULL.  You may **NOT** allocate new ListNodes.
 * Note that since the tail should be continuously updated, some nodes will
 * be moved more than once.
 */
template <typename T>
void List<T>::waterfall() {
  if(head_ == NULL || tail_ == NULL || size() == 1){
    return;
  }
  ListNode * node = head_;
  /// @todo Graded in part 1
  for(int i = 0; node != NULL; i++){
    ListNode * temp = node->next;
    if(i%2==1){//if odd, send to back
      //remove
      if(node->next == NULL){
        break;
      }
      node->prev->next = node->next;
      node->next->prev = node->prev;
      node->prev = tail_;
      node->next = NULL;
      tail_->next = node;  
      tail_ = node;
    }
    node = temp;
  }
}


/**
 * Reverses the current List.
 */
template <typename T>
void List<T>::reverse() {
  reverse(head_, tail_);
}

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
 */
template <typename T>
void List<T>::reverse(ListNode *& startPoint, ListNode *& endPoint) {
  /// @todo Graded in mp_lists part 2
 if (startPoint == NULL || endPoint == NULL|| startPoint == endPoint) {
    return;
  }

  ListNode *prev = NULL;
  ListNode *current = startPoint;
  ListNode *next = NULL;

  while (current != endPoint) {
    next = current->next;
    current->next = prev;
    prev = current;
    current = next;
  }

  startPoint->next = current->next;
  current->next = prev;

  ListNode *temp = startPoint;
  startPoint = endPoint;
  endPoint = temp;
}
/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
 */
template <typename T>
void List<T>::reverseNth(int n) {
/// @todo Graded in mp_lists part 2
  if (n <= 1 || head_ == NULL) {
    return;
  }
  ListNode* current = head_;
  ListNode* prev = NULL;
  while (current != NULL) {
    ListNode* start = current;
    ListNode* end = current;
    int i = 1;
    while (i < n && end->next != NULL) {
      end = end->next;
      i++;
    }
    current = end->next;
    reverse(start, end);
    if (prev == NULL) {
      head_ = start;
    } else {
      prev->next = start;
    }
    prev = end;
  }
}


/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <typename T>
void List<T>::mergeWith(List<T> & otherList) {
    // set up the current list
    head_ = merge(head_, otherList.head_);
    tail_ = head_;

    // make sure there is a node in the new list
    if (tail_ != NULL) {
        while (tail_->next != NULL)
            tail_ = tail_->next;
    }
    length_ = length_ + otherList.length_;

    // empty out the parameter list
    otherList.head_ = NULL;
    otherList.tail_ = NULL;
    otherList.length_ = 0;
}

/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second sequence.
 * @return The starting node of the resulting, sorted sequence.
 */

template <typename T>
typename List<T>::ListNode * List<T>::merge(ListNode * first, ListNode* second) {
  if (first == NULL) {
    return second;
  }
  if (second == NULL) {
    return first;
  }
  ListNode * curr = nullptr;
  ListNode * head = nullptr;
  while(first != nullptr && second != nullptr){
    if(first->data < second->data){
      if(head == nullptr){//check if head is initialized yet
        head = first;
      } else {
        curr->next = first;
      }
      curr = first;
      first = first->next;   
    } else {
      if(head == nullptr){
        head = second;
      } else {
        curr->next = second;
      }
      curr = second;
      second = second->next;
    }
  }
  if(first != nullptr){
    curr->next = first;
  } else {
    curr->next = second;
  }
  return head;
}

/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * Called by the public sort function in List-given.hpp
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */
template <typename T>
typename List<T>::ListNode* List<T>::mergesort(ListNode * start, int chainLength) {
  /// @todo Graded in mp_lists part 2
  if(chainLength <= 1){
    return start;
  }
  ListNode * splitPoint = split(start, chainLength/2);
  start = mergesort(start, chainLength/2);
  splitPoint = mergesort(splitPoint, chainLength - (chainLength/2));
  start = merge(start, splitPoint);
  return start;
}
