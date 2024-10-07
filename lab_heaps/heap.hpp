
/**
 * @file heap.cpp
 * Implementation of a heap class.
 */
#include <cstddef>
using namespace std;

template <class T, class Compare>
class heap;

template <class T, class Compare>
size_t heap<T, Compare>::root() const
{
    // @TODO Update to return the index you are choosing to be your root.
    return 1;
}

template <class T, class Compare>
size_t heap<T, Compare>::leftChild(size_t currentIdx) const
{
    // @TODO Update to return the index of the left child.
    return (2*currentIdx);
}

template <class T, class Compare>
size_t heap<T, Compare>::rightChild(size_t currentIdx) const
{
    // @TODO Update to return the index of the right child.
    return (2*currentIdx + 1);
}

template <class T, class Compare>
size_t heap<T, Compare>::parent(size_t currentIdx) const
{
    // @TODO Update to return the index of the parent.
    return (size_t)currentIdx/2;
}

template <class T, class Compare>
bool heap<T, Compare>::hasAChild(size_t currentIdx) const
{
    // @TODO Update to return whether the given node has a child
    return leftChild(currentIdx) < _elems.size();
}

template <class T, class Compare>
size_t heap<T, Compare>::maxPriorityChild(size_t currentIdx) const
{
    // @TODO Update to return the index of the child with highest priority
    ///   as defined by higherPriority()

    //check if no right child
    if(rightChild(currentIdx) >= _elems.size()){//left child index is the last one
        return leftChild(currentIdx);
    }

    if(higherPriority(_elems[leftChild(currentIdx)], _elems[rightChild(currentIdx)])){
        return leftChild(currentIdx);
    }
    return rightChild(currentIdx);
}

template <class T, class Compare>
void heap<T, Compare>::heapifyDown(size_t currentIdx)
{
    if(!hasAChild(currentIdx)){
        return;
    }
    // @TODO Implement the heapifyDown algorithm.
    size_t maxChild = maxPriorityChild(currentIdx);
    if(higherPriority(_elems[maxChild], _elems[currentIdx])){ //child has higher priority
        swap(_elems[currentIdx], _elems[maxChild]);//maxChild now holds original subroot
        heapifyDown(maxChild);
    }
}

template <class T, class Compare>
void heap<T, Compare>::heapifyUp(size_t currentIdx)
{
    if (currentIdx == root())
        return;
    size_t parentIdx = parent(currentIdx);
    if (higherPriority(_elems[currentIdx], _elems[parentIdx])) {
        swap(_elems[currentIdx], _elems[parentIdx]);
        heapifyUp(parentIdx);
    }
}

template <class T, class Compare>
heap<T, Compare>::heap()
{
    // @TODO Depending on your implementation, this function may or may
    ///   not need modifying
    _elems.push_back(T());
}

template <class T, class Compare>
heap<T, Compare>::heap(const std::vector<T>& elems) 
{
    // @TODO Construct a heap using the buildHeap algorithm
    _elems.push_back(T());
    for(T item : elems){
        _elems.push_back(item);
    }
    for(size_t lol = parent(_elems.size() - 1); lol > 0; lol--){
        heapifyDown(lol);
    }
}

template <class T, class Compare>
T heap<T, Compare>::pop()
{
    // @TODO Remove, and return, the element with highest priority

    //swap with rightmost leaf and relocate
    T ret = _elems[root()];
    size_t right = _elems.size() - 1;
    _elems[root()] = _elems[right];
    _elems.pop_back(); //remove root
    heapifyDown(root());//relocate
    return ret;
}

template <class T, class Compare>
T heap<T, Compare>::peek() const
{
    // @TODO Return, but do not remove, the element with highest priority
    return _elems[root()];
}

template <class T, class Compare>
void heap<T, Compare>::push(const T& elem)
{
    // @TODO Add elem to the heap
    size_t newidx = _elems.size();
    _elems.push_back(elem); //add to vector
    heapifyUp(newidx); //position correctly
}

template <class T, class Compare>
void heap<T, Compare>::updateElem(const size_t & idx, const T& elem)
{
    // @TODO In-place updates the value stored in the heap array at idx
    // Corrects the heap to remain as a valid heap even after update
    T previous = _elems[idx];
    _elems[idx] = elem;
    if(higherPriority(previous, elem)){
        heapifyDown(idx);
    } else {
        heapifyUp(idx);
    }
}


template <class T, class Compare>
bool heap<T, Compare>::empty() const
{
    // @TODO Determine if the heap is empty
    if(_elems.size() <= 1){
        return true;
    }
    return false;
}

template <class T, class Compare>
void heap<T, Compare>::getElems(std::vector<T> & heaped) const
{
    for (size_t i = root(); i < _elems.size(); i++) {
        heaped.push_back(_elems[i]);
    }
}
