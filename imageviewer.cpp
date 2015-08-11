/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#ifndef QT_NO_PRINTER
#include <QPrintDialog>
#endif

#include "imageviewer.h"
#include <QDebug>
#include "imageprocessor.h"


//! [0]
ImageViewer::ImageViewer()
{
    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    setCentralWidget(scrollArea);


    createActions();
    createMenus();

    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}


//! [0]
//! [2]

bool ImageViewer::loadFile(const QString &fileName)
{
    QImage image(fileName);

    if (image.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1.").arg(QDir::toNativeSeparators(fileName)));
        setWindowFilePath(QString());

        imageLabel->setPixmap(QPixmap());
        imageLabel->adjustSize();
        return false;
    }
//    image.convertToFormat(QImage::Format_RGB32);
    tempImage = image.copy();
    loadImage(tempImage);

//! [2] //! [3]
//! [3] //! [4]
    scaleFactor = 1.0;

    printAct->setEnabled(true);
    fitToWindowAct->setEnabled(true);
    updateActions();

    if (!fitToWindowAct->isChecked())
        imageLabel->adjustSize();

    setWindowFilePath(fileName);
    greenOn = true;
    blueOn = true;
    redOn = true;
    return true;
}

//! [4]

//! [2]

//! [1]
void ImageViewer::open()
{
    QStringList mimeTypeFilters;
    foreach (const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes())
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    QFileDialog dialog(this, tr("Open File"),
                       picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.first());
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}
//! [1]

bool ImageViewer::saveFile(const QString &fileName)
{

    if(!(imageLabel->pixmap()->isNull())){
        imageLabel->pixmap()->save(fileName);
        setWindowFilePath(fileName);
    }else{
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1.").arg(QDir::toNativeSeparators(fileName)));
        setWindowFilePath(QString());
        imageLabel->setPixmap(QPixmap());
        imageLabel->adjustSize();
        return false;
    }

    return true;
}

void ImageViewer::saveas(){

        QStringList mimeTypeFilters;
        foreach (const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes())
            mimeTypeFilters.append(mimeTypeName);
        mimeTypeFilters.sort();
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        QFileDialog dialog(this, tr("Save File"),
                           picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.first());
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setDefaultSuffix("jpg");
        dialog.setMimeTypeFilters(mimeTypeFilters);
        dialog.selectMimeTypeFilter("image/jpeg");

        while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles().first())) {}

}

//! [5]
void ImageViewer::print()
//! [5] //! [6]
{
    Q_ASSERT(imageLabel->pixmap());
#if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
//! [6] //! [7]
    QPrintDialog dialog(&printer, this);
//! [7] //! [8]
    if (dialog.exec()) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = imageLabel->pixmap()->size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(imageLabel->pixmap()->rect());
        painter.drawPixmap(0, 0, *imageLabel->pixmap());
    }
#endif
}
//! [8]

//! [9]
void ImageViewer::zoomIn()
//! [9] //! [10]
{
    scaleImage(1.25);
}

void ImageViewer::zoomOut()
{
    scaleImage(0.8);
}

//! [10] //! [11]
void ImageViewer::normalSize()
//! [11] //! [12]
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}
//! [12]

//! [13]
void ImageViewer::fitToWindow()
//! [13] //! [14]
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        normalSize();
    }
    updateActions();
}
//! [14]


//! [15]
void ImageViewer::about()
//! [15] //! [16]
{
    QMessageBox::about(this, tr("About Image Viewer"),
            tr("<p>The <b>Image Viewer</b> example shows how to combine QLabel "
               "and QScrollArea to display an image. QLabel is typically used "
               "for displaying a text, but it can also display an image. "
               "QScrollArea provides a scrolling view around another widget. "
               "If the child widget exceeds the size of the frame, QScrollArea "
               "automatically provides scroll bars. </p><p>The example "
               "demonstrates how QLabel's ability to scale its contents "
               "(QLabel::scaledContents), and QScrollArea's ability to "
               "automatically resize its contents "
               "(QScrollArea::widgetResizable), can be used to implement "
               "zooming and scaling features. </p><p>In addition the example "
               "shows how to use QPainter to print an image.</p> <p> Newly added features by KRV "
               "Colour filters, Histograms, Resampling, Negative"));
}
//! [16]

//! [17]
void ImageViewer::createActions()
//! [17] //! [18]
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveasAct = new QAction(tr("&Save..."), this);
    saveasAct->setShortcut(tr("Ctrl+S"));
    connect(saveasAct, SIGNAL(triggered()), this, SLOT(saveas()));

    cloneWindowAct = new QAction(tr("&Clone..."), this);
    cloneWindowAct->setShortcut(tr("Ctrl+C"));
    connect(cloneWindowAct, SIGNAL(triggered()), this, SLOT(cloneWindow()));

    printAct = new QAction(tr("&Print..."), this);
    printAct->setShortcut(tr("Ctrl+P"));
    printAct->setEnabled(false);
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    zoomInAct = new QAction(tr("Zoom &In (25%)"), this);
    zoomInAct->setShortcut(tr("Ctrl++"));
    zoomInAct->setEnabled(false);
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAct = new QAction(tr("Zoom &Out (25%)"), this);
    zoomOutAct->setShortcut(tr("Ctrl+-"));
    zoomOutAct->setEnabled(false);
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

    normalSizeAct = new QAction(tr("&Normal Size"), this);
    normalSizeAct->setShortcut(tr("Ctrl+N"));
    normalSizeAct->setEnabled(false);
    connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

    fitToWindowAct = new QAction(tr("&Fit to Window"), this);
    fitToWindowAct->setEnabled(false);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));
    connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    redToggleAct = new QAction(tr("&Red Toggle"), this);
    connect(redToggleAct, SIGNAL(triggered()), this, SLOT(redToggle()));

    greenToggleAct = new QAction(tr("&Green Toggle"), this);
    connect(greenToggleAct, SIGNAL(triggered()), this, SLOT(greenToggle()));

    blueToggleAct = new QAction(tr("&Blue Toggle"), this);
    connect(blueToggleAct, SIGNAL(triggered()), this, SLOT(blueToggle()));

    negativeAct = new QAction(tr("&Negative"), this);
    negativeAct->setShortcut(tr("Ctrl+R"));
    connect(negativeAct, SIGNAL(triggered()), this, SLOT(negative()));

    resampleAct = new QAction(tr("&Resample"), this);
    connect(resampleAct, SIGNAL(triggered()), this, SLOT(imageResample()));
}
//! [18]

//! [19]
void ImageViewer::createMenus()
//! [19] //! [20]
{
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveasAct);
    fileMenu->addAction(cloneWindowAct);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    viewMenu = new QMenu(tr("&View"), this);
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);
    viewMenu->addAction(normalSizeAct);
    viewMenu->addSeparator();
    viewMenu->addAction(fitToWindowAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    colourMenu = new QMenu(tr("&Colour Map"), this);
    colourMenu->addAction(redToggleAct);
    colourMenu->addAction(greenToggleAct);
    colourMenu->addAction(blueToggleAct);

    effectsMenu = new QMenu(tr("&Effects"), this);
    effectsMenu->addMenu(colourMenu);
    effectsMenu->addAction(resampleAct);
    effectsMenu->addAction(negativeAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(effectsMenu);
    menuBar()->addMenu(helpMenu);
}
//! [20]

//! [21]
void ImageViewer::updateActions()
//! [21] //! [22]
{
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}
//! [22]

//! [23]
void ImageViewer::scaleImage(double factor)
//! [23] //! [24]
{
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}
//! [24]

//! [25]
void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
//! [25] //! [26]
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}
//! [26]

void ImageViewer::cloneWindow(){
    ImageViewer *img = new ImageViewer();
    img->imageLabel->setPixmap(imageLabel->pixmap()->copy());
    img->imageLabel->adjustSize();
    img->scaleFactor = 1.0;

    img->printAct->setEnabled(true);
    img->fitToWindowAct->setEnabled(true);
    img->updateActions();

    if (!img->fitToWindowAct->isChecked())
        img->imageLabel->adjustSize();
    img->show();
}

/*
 * Load the image to a data structure
 * */
void ImageViewer::loadImage(QImage image){

    imageLabel->setPixmap(QPixmap::fromImage(image));
}

void ImageViewer::redToggle(){
    if(redOn && greenOn && blueOn){
    int num_of_cols = tempImage.height();
    int num_of_rows = tempImage.width();
//    QImage image2(3,3, QImage::Format_RGB32);
    QImage img = tempImage.copy();
    for(int row=0; row<num_of_rows; ++row){
        for(int col=0; col<num_of_cols; ++col){
            QRgb clr = tempImage.pixel(row, col);
            QColor colr = QColor(clr);
            clr = qRgba(colr.red(), 0, 0, 255);
            img.setPixel(row,col,clr);
        }
    }
    imageLabel->setPixmap(QPixmap::fromImage(img));
    redOn = false;
    }else{
        imageLabel->setPixmap(QPixmap::fromImage(tempImage));
//        redToggle();
        redOn = true;
        blueOn = true;
        greenOn = true;
    }
}

void ImageViewer::greenToggle(){
    if(redOn && greenOn && blueOn){
    int num_of_cols = tempImage.height();
    int num_of_rows = tempImage.width();
    QImage img = tempImage.copy();
    for(int row=0; row<num_of_rows; ++row){
        for(int col=0; col<num_of_cols; ++col){
            QRgb clr = img.pixel(row, col);
            QColor colr = QColor(clr);
            clr = qRgba(0,colr.green(),0, 255);
            img.setPixel(row,col,clr);
        }
    }
    imageLabel->setPixmap(QPixmap::fromImage(img));
    greenOn = false;
    }else{
        imageLabel->setPixmap(QPixmap::fromImage(tempImage));
//        greenToggle();
        redOn = true;
        blueOn = true;
        greenOn = true;
    }
}

void ImageViewer::blueToggle(){
    if(redOn && greenOn && blueOn){
    int num_of_cols = tempImage.height();
    int num_of_rows = tempImage.width();
//    QImage image2(3,3, QImage::Format_RGB32);

    QImage img = tempImage.copy();

    for(int row=0; row<num_of_rows; ++row){
        for(int col=0; col<num_of_cols; ++col){
//            if(col > 5 and col < 10) continue;
            QRgb clr = img.pixel(row, col);
            QColor colr = QColor(clr);
            clr = qRgba(0,0,colr.blue(), 255);
            img.setPixel(row,col,clr);
        }
    }
        imageLabel->setPixmap(QPixmap::fromImage(img));
        blueOn = false;
    }else{
        imageLabel->setPixmap(QPixmap::fromImage(tempImage));
        redOn = true;
        blueOn = true;
        greenOn = true;
    }
}

void ImageViewer::imageResample(){
    int num_of_cols = tempImage.height();
    int num_of_rows = tempImage.width();
    QImage img2(num_of_rows/2, num_of_cols/2, QImage::Format_RGB32);

    for(int row = 0 ; row < num_of_rows ; row = row + 2){
        for (int col = 0; col < num_of_cols; col = col + 2) {
            if(row%2 == 0 && col%2 == 0){
                QRgb clr = tempImage.pixel(row,col);
                QColor pixi = QColor(clr);
                clr = qRgba(pixi.red(),pixi.green(),pixi.blue(),pixi.alpha());
                img2.setPixel(row/2,col/2,clr);
            }

        }
    }

     imageLabel->setPixmap(QPixmap::fromImage(img2));
     imageLabel->adjustSize();
     tempImage = img2.copy();
}

void ImageViewer::negative(){
    int num_of_cols = tempImage.height();
    int num_of_rows = tempImage.width();
    QImage img = tempImage.copy();

    for(int row = 0 ; row < num_of_rows ; ++row){
        for (int col = 0; col < num_of_cols; ++col) {
            QRgb clr = tempImage.pixel(row,col);
            QColor pixi = QColor(clr);
            clr = qRgba(255-pixi.red(),255-pixi.green(),255-pixi.blue(),255);
            img.setPixel(row,col,clr);
        }
    }

     imageLabel->setPixmap(QPixmap::fromImage(img));
     imageLabel->adjustSize();
     tempImage = img.copy();
}


