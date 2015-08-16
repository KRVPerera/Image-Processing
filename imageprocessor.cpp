#include "imageprocessor.h"

ImageProcessor::ImageProcessor()
{

}

void ImageProcessor::setImage(QImage img){
    width = img.width();
    height = img.height();
    image.clear();
    for(int row = 0 ; row < width ; ++row){
        for (int col = 0; col < height; ++col) {
            QRgb clr = img.pixel(row, col);
            QColor colr = QColor(clr);
            Pixel pix(0,0,0,0);
            pix.setColour(colr.red(),colr.green(),colr.blue(), colr.alpha());
            image.append(pix);
        }
    }
}

QImage ImageProcessor::getImage(){
    QImage out_image(width, height, QImage::Format_RGB32);
    for(int row = 0 ; row < width ; ++row){
        for (int col = 0; col < height; ++col) {
            QRgb clr ;
            Pixel pixi(image.at(row+col));
            clr = qRgba(pixi.getRed(), pixi.getGreen(), pixi.getBlue(), pixi.getAlpha());
            out_image.setPixel(row,col,clr);
        }
    }


    return out_image;
}

QImage ImageProcessor::toImage(QVector<Pixel> img, int w, int h){
    QImage out_image(w, h, QImage::Format_RGB32);

    for(int row = 0 ; row < w ; ++row){
        for (int col = 0; col < h; ++col) {
            QRgb clr ;
            Pixel pixi = img.at(row+col);
            clr = qRgba(pixi.getRed(), pixi.getGreen(), pixi.getBlue(), pixi.getAlpha());
            out_image.setPixel(row,col,clr);
        }
    }
    return out_image;
}

QVector<Pixel> ImageProcessor::toImageVec(){
    return image;
}



QImage ImageProcessor::nearestNeighbourResample(){

    QVector<Pixel> image_temp;


    for(int row = 0 ; row < height ; ++row){
        for (int col = 0; col < width; ++col) {
            if( row % 5 == 0 || col % 5 == 0){
                continue;
            }
            image_temp.append(image.at(row+col));
        }
    }

    QImage out_image(ImageProcessor::toImage(image_temp, width, height));
//    out_image = new QImage(ImageProcessor::toImage(image_temp, width, height));

    return out_image;
}

void ImageProcessor::setHeight(int h){
    height = h;
}

int ImageProcessor::getHeight(){
    return height;
}

void ImageProcessor::setWidth(int w){
    width = w;
}

int ImageProcessor::getWidth(){
    return width;
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

QImage ImageProcessor::negative(){
    QImage out_image(width, height, QImage::Format_RGB32);

    for(int row = 0 ; row < width ; ++row){
        for (int col = 0; col < height; ++col) {
            QRgb clr ;
            Pixel pixi = image.at(row+col);
            clr = qRgba(255-pixi.getRed(),255-pixi.getGreen(),255-pixi.getBlue(),255);
            out_image.setPixel(row,col,clr);
        }
    }
    setImage(out_image);
    return out_image;
}


