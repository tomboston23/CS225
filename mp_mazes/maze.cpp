/* Your code here! */
#include "maze.h"
#include <iostream>
using namespace std;
using namespace cs225;
#include <algorithm>
#include <queue>


SquareMaze::SquareMaze() {
  
}

void SquareMaze::makeMaze(int width, int height) {
  
    mazeWidth = width;
    mazeHeight = height;
    walle.resize(width * height, 3);
    dset.addelements(width * height);

    while (!(dset.size(0) == width * height)) {//go until entire set of squares is in 1 dset
        int square = rand() %(width*height);
        int wall = rand() % 2;

        if (wall == 0) {
            // Check the right wall
            //check we can go right and not already connected
            if((square%width) < width-1 && dset.find(square)!=dset.find(square+1)){
                setWall(square%width, square/width, 0, false);
                dset.setunion(square, square+1);
            }
      

        } else {
            // Check the bottom wall
            //we can go down and not already connected
            if((square/width < height - 1) && (dset.find(square)!= dset.find(square+width))){
                setWall(square%width, square/width, 1, false);
                dset.setunion(square, square+width);
            }
        }
    }
}

bool SquareMaze::canTravel(int x, int y, int dir) const {

  if (mazeWidth == 0) {
    return false;
  }

  if (mazeHeight == 0) {
    return false;
  }
  int wall = walle[y*mazeWidth+x];
    if(dir == 0){//right
        if(x < mazeWidth-1 && (wall == 0 || wall == 2)){ //no right wall
            return true;
        }
    } else if(dir == 1){//down
        if(y < mazeHeight - 1 && (wall == 0 || wall == 1)){ //no bottom wall 
            return true;
        }
    } else if(dir == 2){//left
        if(x > 0){//check if in range
            int leftWall = walle[y*mazeWidth + (x-1)];
            if(leftWall == 0 || leftWall == 2) return true; //no left wall
        }
    } else if(dir == 3){
        if(y > 0){
            int topWall = walle[(y-1)*mazeWidth + x];
            if(topWall == 0 || topWall == 1) return true; //no top wall
        }
    } else return false; //gave an invalid direction

  return false;
}

void SquareMaze::setWall(int x, int y, int dir, bool exists) {
   int index = y*mazeWidth+x;
   int wall = walle[index];

    if(dir == 0){
        if(exists){
            if(wall == 1 || wall == 3) return; //already there
            else {
                walle[index] += 1; //add right wall
            }
        } else {
            if(wall == 0 || wall == 2) return; //already not there
            else {
                walle[index] -= 1; //remove right wall
            }
        }
    }
    if(dir == 1){
        if(exists){
            if(wall == 2 || wall == 3) return; //bottom wall already there
            else {
                walle[index] += 2; //add bottom wall
            }
        } else {
            if(wall == 0 || wall == 1) return; //battom wall already not there
            else {
                walle[index] -= 2; //remove bottom wall
            }
        }
    }
}

vector<int> SquareMaze::solveMaze() {

    int size = mazeWidth * mazeHeight;
    vector<int> length(size, -1); // push placeholder values; will all get filled in eventually
    queue<int> q;
    vector<int> path(size, -1);
    vector<int> previous(size, -1); 
    q.push(0);//starting square

    while (!q.empty()) { // get distances from starting point
        int index = q.front();
        int x = index%mazeWidth;
        int y = index/mazeWidth;
        for (double dir = 0; dir < 4; dir++) {
            if (canTravel(x, y, dir)) {
                int next_index = getNextIndex(index, dir);
                if (length[next_index] < 0) { //check to see if it's visited; if it is, continue searching
                    length[next_index] = length[index] + 1; // Increment length -> we are 1 further away now
                    path[next_index] = dir;
                    previous[next_index] = index;
                    q.push(next_index);
                }
            }
        }
    q.pop();
    }

    int longestPathIndex = 0;
    int longestDist = 0;

    //find longest length in bottom row
    int y = mazeHeight - 1;
    for (int i = y*mazeWidth; i < size; i++) {//iterate along bottom row
        if (length[i] > longestDist) {
            longestPathIndex = i;
            longestDist = length[i];
        }
    }

    vector<int> result;
    for (; longestPathIndex > 0; longestPathIndex = previous[longestPathIndex]) { 
        result.push_back(path[longestPathIndex]);
        if(longestPathIndex < 0) {
            break;
        }
    }

    reverse(result.begin(), result.end());

    return result;
}

int SquareMaze::getNextIndex(int index, int dir) {
    if(dir == 0) return index +1;
    else if(dir == 1) return index +mazeWidth;
    else if(dir == 2) return index - 1;
    else if(dir == 3) return index - mazeWidth;
    else return index; //invalid index, return original
}

PNG * SquareMaze::drawMaze() const {

    PNG* result = new PNG(mazeWidth * 10 + 1, mazeHeight * 10 + 1);
  
    for (int i = 0; i < mazeWidth * 10 + 1; i++) { //Fill in the top wall first
        if (i < 1 || i > 9) {//leave opening alone
            HSLAPixel & pixel = result->getPixel(i, 0);
            pixel.l = 0;
        }
    }
  
    for (int j = 0; j < mazeHeight * 10 + 1; j++) { //Fill in the left wall
        HSLAPixel & pixel = result->getPixel(0, j);
        pixel.l = 0;
    }
  
    for (int x = 0; x < mazeWidth; x++) { //draw the actual walls
        for (int y = 0; y < mazeHeight; y++) {
            int wall = walle[y*mazeWidth+x];
            if (wall == 1 || wall == 3) {//right wall exists
                for (int k = 0; k <= 10; k++) {
                    HSLAPixel & pixel = result->getPixel((x+1)*10, y*10+k);
                    pixel.l = 0;
                }
            }
            if (wall == 2 || wall == 3) {//bottom wall exists
                for (int k = 0; k <= 10; k++) {
                    HSLAPixel & pixel = result->getPixel(x*10+k, (y+1)*10);
                    pixel.l = 0;
                }
            }
        }
    }
    return result;
}

PNG * SquareMaze::drawMazeWithSolution() {
  
    PNG* result = drawMaze();
    vector<int> solution = solveMaze();

    int x = 5;
    int y = 5;

    size_t path = solution.size();

    for (size_t i = 0; i < path; i++) {
        int d = solution[i];
        for (int j = 0; j < 10; j++) {

            //draw red
            HSLAPixel & pixel = result->getPixel(x, y);
            pixel.h = 0;
            pixel.s = 1;
            pixel.l = 0.5;
            pixel.a = 1;

            if(d == 0) x+=1;
            else if(d == 1) y+=1;
            else if(d == 2) x-=1;
            else if(d == 3) y-=1;
        }
    }
    //color last pixel
    HSLAPixel & pixel = result->getPixel(x, y);
    pixel.h = 0;
    pixel.s = 1;
    pixel.l = 0.5;
    pixel.a = 1;
  
    x -= 4;
    y += 5;

    //open bottom wall at end
    for (int k = 0; k < 9; k++) {
        HSLAPixel & pixel = result->getPixel(x + k, y);
        pixel.l = 1;
    }

    return result;
}

