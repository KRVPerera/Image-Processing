#include "pixel.h"

Pixel::Pixel()
{

}

Pixel::Pixel(int r, int g, int b){
    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;
}

QColor Pixel::getQColor(){
    QColor new_colour(rgb[0],rgb[1],rgb[2]);
    return new_colour;
}

int* Pixel::getColours(){
    return rgb;
}

int Pixel::getRed(){
    return rgb[0];
}

int Pixel::getGreen(){
    return rgb[1];
}

int Pixel::getBlue(){
    return rgb[2];
}

void Pixel::setColour(int r, int g, int b){
    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;
}
