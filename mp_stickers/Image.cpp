#include "Image.h"
#include "cs225/HSLAPixel.h"
#include <iostream>

using cs225::PNG;
using cs225::HSLAPixel;
using namespace std;

void Image::lighten(){
    for (unsigned int i = 0; i<width(); i++){
        for (unsigned int j = 0; j < height(); j++){
            HSLAPixel &p = getPixel(i,j);
            if(p.l > 0.9){
                p.l = 1;
            } else {
                p.l += 0.1;
            }
        }
    }
}
void Image::lighten(double amount){
    for (unsigned int i = 0; i<width(); i++){
        for (unsigned int j = 0; j < height(); j++){
            HSLAPixel &p = getPixel(i,j);
            if(p.l > 1-amount){
                p.l = 1;
            } else {
                p.l += amount;
            }
        }
    }
}

void Image::darken(){
    for(unsigned int i = 0; i<width(); i++){
        for(unsigned int j=0; j<height(); j++){
            HSLAPixel &p = getPixel(i,j);
            if(p.l < 0.1){
                p.l = 0;
            } else {
                p.l -= 0.1;
            }
        }
    }
}
void Image::darken(double amount){
    for(unsigned int i = 0; i<width(); i++){
        for(unsigned int j=0; j<height(); j++){
            HSLAPixel &p = getPixel(i,j);
            if(p.l < amount){
                p.l = 0;
            } else {
                p.l -= amount;
            }
        }
    }
}
void Image::saturate(){
    for(unsigned int i = 0; i<width(); i++){
        for(unsigned int j=0; j<height(); j++){
            HSLAPixel &p = getPixel(i,j);
            if(p.s > 0.9){
                p.s = 1;
            } else {
                p.s += 0.1;
            }
        }
    }
}
void Image::saturate(double amount){
    for(unsigned int i = 0; i<width(); i++){
        for(unsigned j=0; j<height(); j++){
            HSLAPixel &p = getPixel(i,j);
            if(p.s > 1-amount){
                p.s = 1;
            } else {
                p.s += amount;
            }
        }
    }
}
void Image::desaturate(){
    for(unsigned int i = 0; i<width(); i++){
        for(unsigned int j=0; j<height(); j++){
            HSLAPixel &p = getPixel(i,j);
            if(p.s < 0.1){
                p.s = 0;
            } else {
                p.s -= 0.1;
            }
        }
    }
}
void Image::desaturate(double amount){
    for(unsigned int i = 0; i<width(); i++){
        for(unsigned int j=0; j<height(); j++){
            HSLAPixel &p = getPixel(i,j);
            if(p.s < amount){
                p.s = 0;
            } else {
                p.s -= amount;
            }
        }
    }
}
void Image::grayscale(){
    for(unsigned int i = 0; i<width(); i++){
        for(unsigned int j=0; j<height(); j++){
            HSLAPixel &p = getPixel(i,j);
            p.s = 0;
        }
    }
}
void Image::rotateColor(double degrees){
    for(unsigned int i = 0; i<width(); i++){
        for(unsigned int j=0; j<height(); j++){
            HSLAPixel &p = getPixel(i,j);
            double hue = p.h;
            hue += degrees;
            //correct to make sure hue is in the range [0,360]
            while (hue < 0){
                hue += 360;
            }
            while (hue > 360){
                hue -= 360;
            }
            p.h = hue;
        }
    }
}

void Image::illinify(){
    for(unsigned int i = 0; i<width(); i++){
        for(unsigned int j=0; j<height(); j++){
            HSLAPixel &p = getPixel(i,j);
            double hue = p.h;
            double orange = 11, blue = 216;
            double orange_diff, blue_diff;
            double diff1, diff2;
            if(hue > orange){
                diff1 = hue - orange;
                diff2 = 360 - diff1;
            } else {
                diff1 = orange - hue;
                diff2 = 360 - diff1;
            }
            if(diff1 > diff2){
                orange_diff = diff2;
            } else {
                orange_diff = diff1;
            }
            if(hue > blue){
                diff1 = hue - blue;
                diff2 = 360 - diff1;
            } else {
                diff1 = blue - hue;
                diff2 = 360 - diff1;
            }
            if(diff1 > diff2){
                blue_diff = diff2;
            } else {
                blue_diff = diff1;
            }
            if(orange_diff < blue_diff){
                hue = orange;
            } else {
                hue = blue;
            }
            p.h = hue;
        }
    }
}

void Image::scale(double factor){
    //create a new image to store the scaled image while leaving the old one intact
    unsigned int new_width = width() * factor;
    unsigned int new_height = height() * factor;
    Image copy = Image(*this);
    resize(new_width, new_height);
    //actually scale the image
    for (unsigned i = 0; i < width(); i++){
        for (unsigned j = 0; j < height(); j++){
            unsigned new_i = i/factor;
            unsigned new_j = j/factor;
            HSLAPixel& cock = copy.getPixel(new_i,new_j);
            HSLAPixel& cum = getPixel(i,j);
            cum = cock;
        }
    }
}

void Image::scale(unsigned w, unsigned h){

    double w_scale = (double)w/((double)width());
    double h_scale = (double)h/((double)height());
    if (w_scale < h_scale){
        scale(w_scale);
    } else {
        scale(h_scale);
    }
}