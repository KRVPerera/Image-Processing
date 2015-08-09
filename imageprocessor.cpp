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

