#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QVector>
#include <QImage>
#include <QPoint>
#include <QDebug>

//#include <main

#include "pixel.h"
#include "imageprocessor.h"
//#include "imageviewer.h"

class ImageProcessor
{
private:
    QVector <Pixel> image;
    int width;
    int height;
public:
    ImageProcessor();
    void setImage(QImage);
    QVector<Pixel> toImageVec();
    QImage getImage();
    QImage toImage(QVector<Pixel>, int , int );
    void setWidth(int);
    int getWidth();
    void setHeight(int);
    int getHeight();
    QVector<QPoint> getN4neighbours(int, int);
    QVector<QPoint> getNDneighbours(int, int);
    QVector<QPoint> getN8neighbours(int, int);
    QImage nearestNeighbourResample();
    QImage negative();

};

#endif // IMAGEPROCESSOR_H
