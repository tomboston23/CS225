/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <deque>

#include <cmath>
using namespace std;

template <int Dim>
bool smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim)
{
    /**
     * @todo Implement this function!
     */
    if(first[curDim] < second[curDim]){
      return true;
    } else if (first[curDim] == second[curDim]){
      return (first < second);
    }
    return false;
}

template <int Dim>
bool shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential)
{
    /**
     * @todo Implement this function!
     */
    int currentBestDistance_squared = 0;
    int potentialDistance_squared = 0;
    for(unsigned i = 0; i < Dim; i++){
      currentBestDistance_squared += (target[i]-currentBest[i])*(target[i]-currentBest[i]);
      potentialDistance_squared += (target[i]-potential[i])*(target[i]-potential[i]);
    }
    if(potentialDistance_squared < currentBestDistance_squared){
      return true;
    } else if(potentialDistance_squared == currentBestDistance_squared) {
      return potential < currentBest;
    }
    return false;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
  if(Dim == 0){
    root = nullptr;
    return;
  }
  vector<Point<Dim>> points(newPoints);
  size = newPoints.size();
  root = build(points, 0, points.size() - 1, 0);
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
  copy(this->root, other.root);
  size = other.size();
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::build(vector<Point<Dim>> &points, int leftIndex, int rightIndex, int depth) {
    if (rightIndex < leftIndex) 
    {
    return nullptr; 
    }
    int medianIdx = (leftIndex + rightIndex) / 2;
    
    auto cmp = [d = depth](const auto& l, const auto& r) {
      return smallerDimVal(l, r, d);
    };
  
    select(points.begin() + leftIndex, points.begin() + rightIndex + 1, points.begin() + medianIdx , cmp);

    //create a tree starting from the midpoint
    KDTreeNode* subroot = new KDTreeNode(points[medianIdx]);

    //create left and right subtrees
    subroot->left = build(points, leftIndex, medianIdx - 1, (depth + 1) % Dim);
    subroot->right = build(points, medianIdx + 1, rightIndex, (depth + 1) % Dim);
    return subroot;
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */
    if (this != &rhs){
      delete *this;
      this = new KDTree(rhs);
    }

  return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
    clear(root);
}

template <int Dim>
void KDTree<Dim>::clear(KDTreeNode* node) {
    if (node != nullptr) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}


//distance helper function, finds the square distance between 2 points
template <int Dim>
double KDTree<Dim>::distance(Point<Dim> a, Point<Dim> b) const{
  double d = 0;
  for (int i = 0; i < Dim; ++i) {
    d += (b[i] - a[i]) * (b[i] - a[i]);
  }
  return d;
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearesthelper(const Point<Dim>& query, int dim, KDTreeNode* currRoot) const {
    if (currRoot->left == NULL && currRoot->right == NULL) {
        return currRoot->point;
    }

    Point<Dim> nearest;
    if (smallerDimVal(query, currRoot->point, dim)){
        if (currRoot->left) {
            nearest = findNearesthelper(query, (dim + 1) % Dim, currRoot->left);
        } else {
            nearest = currRoot->point;
        }
    } else {
        if (currRoot->right) {
            nearest = findNearesthelper(query, (dim + 1) % Dim, currRoot->right);
        } else {
            nearest = currRoot->point;
        }
    }

    if (shouldReplace(query, nearest, currRoot->point)) {
        nearest = currRoot->point;
    }

    double radius = distance(query, nearest);
    double splitDist = pow(query[dim] - currRoot->point[dim], 2);

    if (radius >= splitDist) {
        Point<Dim> tempNearest;
        if (smallerDimVal(query, currRoot->point, dim)) {
            if (currRoot->right) {
                tempNearest = findNearesthelper(query, (dim + 1) % Dim, currRoot->right);
            } else {
                tempNearest = currRoot->point;
            }
        } else {
            if (currRoot->left) {
                tempNearest = findNearesthelper(query, (dim + 1) % Dim, currRoot->left);
            } else {
                tempNearest = currRoot->point;
            }
        }

        if (shouldReplace(query, nearest, tempNearest)) {
            nearest = tempNearest;
        }
    }

    return nearest;
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const {

    if(root == NULL){
    return Point<Dim>();
    }
    else {
    return findNearesthelper(query, 0, root);
    }
}

template <typename RandIter, typename Comparator>
void select(RandIter start, RandIter end, RandIter k, Comparator cmp)
{
    /**
     * @todo Implement this function!
     */    
     
     if(start == (end-1)) {
      return;
     }

     RandIter pivotIndex = start + floor(rand() % ((end-1) - start +1));

     pivotIndex = split(start, (end-1), pivotIndex, cmp);

     if(k == pivotIndex){
      return;
     }
     else if(k < pivotIndex){
        return select(start, pivotIndex, k, cmp);
     }
     else {
     return select (pivotIndex + 1, end, k ,cmp);
     }
}

template <typename RandIter, typename Comparator>
RandIter split(RandIter start1, RandIter end1, RandIter k1, Comparator cmp1)
{
    auto pivot = *k1;
    std::swap(*k1,*end1); //CEHCK

   RandIter save = start1;
for(RandIter i= start1; i != (end1) ; i++){
  if(cmp1(*i,pivot)){
    std::swap(*save,*i); //CHECK
    save++;
  }   
}
std::swap(*end1,*save); //CHECK
return save;
}

