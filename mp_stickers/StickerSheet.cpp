#include "StickerSheet.h"
#include <iostream>
using namespace std;

StickerSheet::StickerSheet(const Image& picture){
    //copy picture and put into base
    base = Image(picture.width(), picture.height());
    for (unsigned i = 0; i < picture.height(); i++){
        for (unsigned j = 0; j < picture.width(); j++){
            const HSLAPixel& pix = picture.getPixel(j,i);
            HSLAPixel p = HSLAPixel(pix.h, pix.s, pix.l); //copy pixel
            HSLAPixel& new_pixel = base.getPixel(j,i);
            new_pixel = p; //insert copied pixel into grid
        }
    }
    //initialize vectors
    layerVector = {};
    x_coords = {};
    y_coords = {};
    stickers = {};
}

int StickerSheet::addSticker(Image& sticker, int x, int y){
    int i = 0;
    //find lowest unoccupied layer
    for (int j : layerVector){
        if (j == i){
            i++;
            continue;
        }
    }
    //actually set the sticker
    return setStickerAtLayer(sticker, i, x, y);
}

int StickerSheet::setStickerAtLayer(Image& sticker, unsigned layer, int x, int y){
    bool layerFree = true;
    unsigned long i = 0;
    for (i = 0; i < layerVector.size(); i++){
        if(layer == layerVector[i]){
            layerFree = false;
            break;
        }
    }
    if(!layerFree){
        stickers[i] = &sticker;
        x_coords[i] = x;
        y_coords[i] = y;
        return layer;
    }

    //find correct index of vector for insertion
    //first check to see if it's empty
    if (layerVector.size()==0){
        layerVector.insert(layerVector.begin(), 0);
        x_coords.insert(x_coords.begin(), x);
        y_coords.insert(y_coords.begin(), y);
        stickers.insert(stickers.begin(), &sticker);
        return 0;
    }

    for (i = 0; i < layerVector.size(); i++){
        if (layer < layerVector[i]){
            layerVector.insert(layerVector.begin()+i, layer);
            x_coords.insert(x_coords.begin()+i, x);
            y_coords.insert(y_coords.begin()+i, y);
            stickers.insert(stickers.begin()+i, &sticker);
            return layer;
        }
    }
    //push to the back of the vector
    layerVector.push_back(layerVector.size());
    x_coords.push_back(x);
    y_coords.push_back(y);
    stickers.push_back(&sticker);
    int ret_val = layerVector.back();
    return ret_val;
}

bool StickerSheet::translate(unsigned index, int x, int y){
    bool index_found = false;
    unsigned long i;
    for (i = 0; i < layerVector.size(); i++){
        if (index == layerVector[i]){
            index_found = true;
            break;
        }
    }
    if(!index_found){
        return false;
    }
    x_coords[i] = x; y_coords[i] = y; //update location of sticker
    return true;
}

void StickerSheet::removeSticker(unsigned index){
    unsigned long i = 0;
    bool index_found = false;
    for (i = 0; i < layerVector.size(); i++){
        if (layerVector[i] == index){
            index_found = true;
            break;
        }
    }
    if (!index_found){
        return;
    }
    layerVector.erase(layerVector.begin()+i);
    stickers.erase(stickers.begin()+i);
    x_coords.erase(x_coords.begin()+i);
    y_coords.erase(y_coords.begin()+i);
}

Image* StickerSheet::getSticker(unsigned index){
    unsigned long i;
    bool index_found = false;
    for (i = 0; i < layerVector.size(); i++){
        if (layerVector[i] == index){
            index_found = true;
            break;
        }
    }
    if (!index_found){
        return NULL;
    }
    return stickers[i];
}
int StickerSheet::layers() const{
    return layerVector.size();
}

//big daddy render function
Image StickerSheet::render() const{
    //calculate width and height
    //set default width
    int width = base.width();

    //find adjusted width
    unsigned long i = 0;
    int x_min = 0;
    unsigned x_max = width;
    while (i < layerVector.size()){
        if (x_coords[i] < x_min){
            x_min = x_coords[i];
        }
        if(x_coords[i] + (stickers[i])->width() > x_max){
            x_max = x_coords[i] + (stickers[i])->width();
        }
        width = x_max - x_min;
        i++;
    }

    //set default height
    int height = base.height();

    //find adjusted height
    i = 0;
    int y_min = 0;
    unsigned y_max = height;
    while (i < layerVector.size()){
        if (y_coords[i] < y_min){
            y_min = y_coords[i];
        }
        if(y_coords[i] + (stickers[i])->height() > y_max){
            y_max = y_coords[i] + (stickers[i])->height();
        }
        height= y_max - y_min;
        i++;
    }
    
    Image image = Image(width, height);
    //insert base image (layer -1)
    for (unsigned i = 0; i < base.width(); i++){
        for (unsigned j = 0; j < base.height(); j++){
            unsigned x = i - x_min;
            unsigned y = j - y_min;
            HSLAPixel& pixel = image.getPixel(x, y);
            const HSLAPixel& orig = base.getPixel(i,j);
            pixel = orig;
        }
    }

    //insert layers
    for(unsigned long i = 0; i < layerVector.size(); i++){
        unsigned w = (stickers[i])->width();
        unsigned h = (stickers[i])->height();
        for (unsigned j = 0; j < w; j++){
            for (unsigned k = 0; k < h; k++){
                unsigned x = x_coords[i] - x_min;
                unsigned y = y_coords[i] - y_min;
                HSLAPixel& image_pixel = image.getPixel(j+x, k+y);
                HSLAPixel& sticker_pixel = stickers[i]->getPixel(j,k);
                if(sticker_pixel.a == 1){
                    image_pixel = sticker_pixel;
                }
            }
        }
    }
    return image;
}