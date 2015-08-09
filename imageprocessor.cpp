#include "imageprocessor.h"

ImageProcessor::ImageProcessor()
{

}

void ImageProcessor::setImage(QImage img){
    width = img.width();
    height = img.height();

    for(int row = 0 ; row < height ; ++row){
        for (int col = 0; col < width; ++col) {
            QRgb clr = img.pixel(col, row);
            QColor colr = QColor(clr);
//            clr = qRgba(colr.red(),colr.green(),0, 255);
            Pixel *pix = new Pixel(0,0,0);
            pix->setColour(colr.red(),colr.green(),colr.blue());
            image.append(*pix);
        }
    }
}

QImage& ImageProcessor::getImage(){
    QImage *out_image = new QImage(width, height, QImage::Format_RGB32);
    for(int row = 0 ; row < height ; ++row){
        for (int col = 0; col < width; ++col) {
            QRgb clr ;
            Pixel pixi = image.at(row+col);
            clr = qRgba(pixi.getRed(),pixi.getGreen(),pixi.getBlue(), 255);
            out_image->setPixel(col,row,clr);
        }
    }
    return *out_image;
}

QImage& ImageProcessor::toImage(QVector<Pixel> &img, int w, int h){
    QImage *out_image = new QImage(w, h, QImage::Format_RGB32);
    for(int row = 0 ; row < w ; ++row){
        for (int col = 0; col < h; ++col) {
            QRgb clr ;
            Pixel pixi = img.at(row+col);
            clr = qRgba(pixi.getRed(),pixi.getGreen(),pixi.getBlue(), 255);
            out_image->setPixel(row,col,clr);
        }
    }
    return *out_image;
}




QImage& ImageProcessor::nearestNeighbourResample(){
    QImage *out_image;
    QVector<Pixel> image_temp;
//    image_temp << image;

    for(int row = 0 ; row < height ; ++row){
        for (int col = 0; col < width; ++col) {
            if( row % 2 == 0 || col % 2 == 0){
                continue;
            }
            image_temp.append(image.at(row+col));
        }
    }
//    out_image = new QImage(toImage(image_temp, width/2, height/2));
    out_image = new QImage(toImage(image, width, height));
//    out_image = new QImage(image);
    return *out_image;
}

void ImageProcessor::setHeight(int h){
    height = h;
}

void ImageProcessor::setWidth(int w){
    width = w;
}

QVector<QPoint> ImageProcessor::getN4neighbours(int row, int col){
    QVector<QPoint> neighbours;
    int rows = row - 1;
    int rowe = row + 2;
    int cols = col -1;
    int cole = col + 2;

    if( row <= 0){
        rows = 0;
    }
    if(col <= 0){
        cols = 0;
    }
    if( row >= height - 1 ){
        rowe = height;
    }

    if( col >= width - 1 ){
        cole = width;
    }

    for(int r = rows; r < rowe; ++r){
        for(int c = cols; c < cole; ++c){
            if( (c == col || r == row) && ( c != col || r != row)){
            QPoint *p = new QPoint(r,c);
            neighbours.append(*p);
            }
        }
    }
    return neighbours;
}

