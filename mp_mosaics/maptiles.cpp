/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>
#include "maptiles.h"
//#include "cs225/RGB_HSL.h"

using namespace std;


Point<3> convert(LUVAPixel pixel) {
    return Point<3>(pixel.l, pixel.u, pixel.v);
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    int rows = theSource.getRows();
    int cols = theSource.getColumns();
    Point<3> region, nearestColor, averageColor;
    MosaicCanvas * canvas = new MosaicCanvas(rows, cols);
    vector<Point<3>> pts; //store average colors
    map<Point<3>, TileImage*> averages;
    for (auto it = theTiles.begin(); it != theTiles.end(); ++it) {
        averageColor = convert(it->getAverageColor());
        //assign each index to a tile
        averages[averageColor] = &(*it);
        pts.push_back(averageColor);
    } 
    //make a k-d tree of average colors
    KDTree<3> kdtree(pts);
    //iterate through source image
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            region = convert(theSource.getRegionColor(r, c));
            nearestColor = kdtree.findNearestNeighbor(region);
            canvas->setTile(r, c, averages[nearestColor]);
        }
    }
    return canvas;
}
