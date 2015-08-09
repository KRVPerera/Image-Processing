#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QVector>
#include <QImage>
#include <QPoint>
#include <QDebug>

#include "pixel.h"

class ImageProcessor
{
private:
    QVector <Pixel> image;
    int width;
    int height;
public:
    ImageProcessor();
    void setImage(QImage);
    QImage &getImage();
    QImage &toImage(QVector<Pixel> &, int , int );
    void setWidth(int);
    void setHeight(int);
    QVector<QPoint> getN4neighbours(int, int);
    QVector<QPoint> getNDneighbours(int, int);
    QVector<QPoint> getN8neighbours(int, int);
    QImage& nearestNeighbourResample();

};

#endif // IMAGEPROCESSOR_H
