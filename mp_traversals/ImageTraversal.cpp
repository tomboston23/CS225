#include <cmath>
#include <iterator>
#include <iostream>

#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"
#include "Point.h"

#include "ImageTraversal.h"


namespace Traversals {
  /**
  * Calculates a metric for the difference between two pixels, used to
  * calculate if a pixel is within a tolerance.
  *
  * @param p1 First pixel
  * @param p2 Second pixel
  * @return the difference between two HSLAPixels
  */
  double calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
    double h = fabs(p1.h - p2.h);
    double s = p1.s - p2.s;
    double l = p1.l - p2.l;

    // Handle the case where we found the bigger angle between two hues:
    if (h > 180) { h = 360 - h; }
    h /= 360;

    return sqrt((h*h) + (s*s) + (l*l));
  }
  
  /**
  * Adds a Point for the bfs traversal to visit at some point in the future.
  * @param work_list the deque storing a list of points to be processed
  * @param point the point to be added
  */
  void bfs_add(std::deque<Point> & work_list, const Point & point) {
    /** @todo [Part 1] */
    work_list.push_back(point);
  }

  /**
  * Adds a Point for the dfs traversal to visit at some point in the future.
  * @param work_list the deque storing a list of points to be processed
  * @param point the point to be added
  */
  void dfs_add(std::deque<Point> & work_list, const Point & point) {
    /** @todo [Part 1] */
    work_list.push_front(point);
  }

  /**
  * Removes the current Point in the bfs traversal
  * @param work_list the deque storing a list of points to be processed
  */
  void bfs_pop(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    if(!work_list.empty()){
      work_list.pop_front();
    }
  }

  /**
  * Removes the current Point in the dfs traversal
  * @param work_list the deque storing a list of points to be processed
  */
  void dfs_pop(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    if(!work_list.empty()){
      work_list.pop_front();
    }
  }

  /**
   * Returns the current Point in the bfs traversal
   * @param work_list the deque storing a list of points to be processed
   * @return the current Point 
   */
  Point bfs_peek(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    if(!work_list.empty()){
      return work_list.front();
    }
    return Point(0,0);
  }

  /**
   * Returns the current Point in the dfs traversal
   * @param work_list the deque storing a list of points to be processed
   * @return the current Point 
   */
  Point dfs_peek(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    if(!work_list.empty()){
      return work_list.front();
    }
    return Point(0,0);
  }

  /**
  * Initializes a ImageTraversal on a given `png` image,
  * starting at `start`, and with a given `tolerance`.
  * @param png The image this traversal is going to traverse
  * @param start The start point of this traversal
  * @param tolerance If the current point is too different (difference larger than tolerance) with the start point,
  * it will not be included in this traversal
  * @param fns the set of functions describing a traversal's operation
  */
  ImageTraversal::ImageTraversal(const PNG & png, const Point & start, double tolerance, TraversalFunctions fns) {  
    /** @todo [Part 1] */
    adder = fns.add;
    remover = fns.pop;
    peeper = fns.peek;
    png_ = png;
    startingPoint = start;
    tolerance_ = tolerance;
  }

  ImageTraversal::Iterator::Iterator(const PNG & png, const Point & start, double tolerance, TraversalFunctions fns){
    png_ = png;
    startingPoint = start;
    tolerance_ = tolerance;
    adder = fns.add;
    remover = fns.pop;
    peeper = fns.peek;
    //set visited
    visited.resize(png.height(), vector<bool>(png.width(),false));
    adder(work_list_, start);
  }

  /**
  * Returns an iterator for the traversal starting at the first point.
  */
  ImageTraversal::Iterator ImageTraversal::begin() {
    /** @todo [Part 1] */
    TraversalFunctions functions;
    functions.add = adder;
    functions.pop = remover;
    functions.peek = peeper;
    return ImageTraversal::Iterator(png_, startingPoint, tolerance_, functions);
  }

  /**
  * Returns an iterator for the traversal one past the end of the traversal.
  */
  ImageTraversal::Iterator ImageTraversal::end() {
    /** @todo [Part 1] */
    return ImageTraversal::Iterator();
  }

  /**
  * Default iterator constructor.
  */
  ImageTraversal::Iterator::Iterator() {
    /** @todo [Part 1] */
  }


  /**
  * Iterator increment operator.
  *
  * Advances the traversal of the image.
  */
  ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
    /** @todo [Part 1] */
    if(empty()){
      return *this;
    }

    Point poopy = peeper(work_list_);
    remover(work_list_);

    visited[poopy.y][poopy.x] = true;

    //add neighbors to work list
    Point right = Point(poopy.x + 1, poopy.y);
    Point down = Point(poopy.x, poopy.y + 1);
    Point lefty = Point(poopy.x - 1, poopy.y);
    Point up = Point(poopy.x, poopy.y - 1);
    std::vector<Point> points = {right, down, lefty, up};

    for(Point LLL : points){
      if(isValid(LLL)){
        adder(work_list_, LLL);
      }
    }
    //remove visited locations

    while(!empty()){
      Point front = peeper(work_list_);
      if(visited[front.y][front.x]){
        remover(work_list_);
      } else {
        break;
      }
    }
    return *this;
  }

  /**
  * Iterator accessor operator.
  *
  * Accesses the current Point in the ImageTraversal.
  */
  Point ImageTraversal::Iterator::operator*() {
    /** @todo [Part 1] */
    return peeper(work_list_);
  }

  /**
  * Iterator inequality operator.
  *
  * Determines if two iterators are not equal.
  */
  bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
    /** @todo [Part 1] */
    if(this->work_list_ != other.work_list_){
      return true;
    }
    return false;
  }

  /**
   * Iterator size function.
   *
   * @return size_t the size of the iterator work queue.
   */
  size_t ImageTraversal::Iterator::size() const {
    return work_list_.size();
  }

  /**
   * Iterator empty function.
   *
   * @return bool whether the iterator work queue is empty.
   */
  bool ImageTraversal::Iterator::empty() const {
    return work_list_.empty();
  }

  bool ImageTraversal::Iterator::isValid(const Point & point){
    unsigned x = point.x;
    unsigned y = point.y;

    //check x val
    if(x >= png_.width()){
      return false;
    }
    //check y val
    if(y >= png_.height()){
      return false;
    }
    //check distance between start and point
    double delta = calculateDelta(png_.getPixel(x, y), png_.getPixel(startingPoint.x, startingPoint.y));
    if(delta > tolerance_){
      return false;
    }

    if(visited[y][x]){
      return false;
    }
    return true;
  }
}