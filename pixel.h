#ifndef PIXEL_H
#define PIXEL_H

#include <QRgb>
#include <QColor>

class Pixel
{
private:
    int rgb[3];
public:
    Pixel();
    Pixel(int r, int g, int b);
    void setColour(int r, int g, int b);
    QColor getQColor();
    int* getColours();
    int getRed();
    int getGreen();
    int getBlue();
};

#endif // PIXEL_H
