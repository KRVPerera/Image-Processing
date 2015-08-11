#ifndef PIXEL_H
#define PIXEL_H

#include <QRgb>
#include <QColor>

class Pixel
{
private:
    uchar rgb[4];
public:
    Pixel();
    Pixel(uchar r, uchar g, uchar b, uchar a);
    void setColour(uchar r, uchar g, uchar b, uchar a);
    QColor getQColor();
    uchar* getColours();
    uchar getRed();
    uchar getGreen();
    uchar getBlue();
    uchar getAlpha();
//    Pixel copy();
};

#endif // PIXEL_H
