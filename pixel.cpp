#include "pixel.h"

Pixel::Pixel()
{

}

Pixel::Pixel(uchar r, uchar g, uchar b, uchar a){
    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;
    rgb[3] = a;
}

QColor Pixel::getQColor(){
    QColor new_colour(rgb[0],rgb[1],rgb[2], rgb[3]);
    return new_colour;
}

uchar* Pixel::getColours(){
    return rgb;
}

uchar Pixel::getRed(){
    return rgb[0];
}

uchar Pixel::getGreen(){
    return rgb[1];
}

uchar Pixel::getBlue(){
    return rgb[2];
}

uchar Pixel::getAlpha(){
    return rgb[3];
}

void Pixel::setColour(uchar r, uchar g, uchar b, uchar a){
    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;
    rgb[3] = a;
}

