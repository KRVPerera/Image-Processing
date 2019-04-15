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

#include <QPushButton>
#include <QSlider>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QtMath>
#include <string>

#include "imageviewer.h"
#include "qcustomplot.h"
#include "contraststretchingdialog.h"
//#include "normalizesizingdialog.h"
#include "resamplingscaledialog.h"
#include "huffman.cpp"
#include "customdialog.h"
#include "customfilter.h"



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
    openedFileName = fileName;

    if (image.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1. ").arg(QDir::toNativeSeparators(fileName)));
        setWindowFilePath(QString());

        imageLabel->setPixmap(QPixmap());
        imageLabel->adjustSize();
        return false;
    }
    image.convertToFormat(QImage::Format_RGB32);
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
    brightness = 100;
    contrast = 0;
    backupImage = QImage();
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
    // redToggleAct->setEnabled(false);
    connect(redToggleAct, SIGNAL(triggered()), this, SLOT(redToggle()));

    greenToggleAct = new QAction(tr("&Green Toggle"), this);
    // greenToggleAct->setEnabled(false);
    connect(greenToggleAct, SIGNAL(triggered()), this, SLOT(greenToggle()));

    blueToggleAct = new QAction(tr("&Blue Toggle"), this);
    // blueToggleAct->setEnabled(false);
    connect(blueToggleAct, SIGNAL(triggered()), this, SLOT(blueToggle()));

    negativeAct = new QAction(tr("&Negative"), this);
    negativeAct->setShortcut(tr("Ctrl+R"));
    // negativeAct->setEnabled(false);
    connect(negativeAct, SIGNAL(triggered()), this, SLOT(negative()));

    brightnessContrastAct = new QAction(tr("&Brightness"), this);
    // brightnessContrastAct->setEnabled(false);
    brightnessContrastAct->setShortcut(tr("Ctrl+B"));
    connect(brightnessContrastAct, SIGNAL(triggered()), this, SLOT(brightnessContrast()));

    resampleAct = new QAction(tr("&Resample"), this);
    // resampleAct->setEnabled(false);
    connect(resampleAct, SIGNAL(triggered()), this, SLOT(imageResample()));

    histogramsAct = new QAction(tr("&Histograms"), this);
    connect(histogramsAct, SIGNAL(triggered()), this, SLOT(showHistograms()));

    normalizationAct = new QAction(tr("&Normalize"), this);
    connect(normalizationAct, SIGNAL(triggered()), this, SLOT(normalization()));

    grayImageAct = new QAction(tr("Gray Image"), this);
    connect(grayImageAct, SIGNAL(triggered()), this, SLOT(grayImage()));


    bitPlaneRunLengthCodingAct = new QAction(tr("Bit Plane Run Length Coding"), this);
    connect(bitPlaneRunLengthCodingAct, SIGNAL(triggered()), this, SLOT(bitPlaneRunLengthCoding()));

    logFilterFiveByFiveAct = new QAction(tr("5x5 LOG"), this);
    connect(logFilterFiveByFiveAct, SIGNAL(triggered()), this, SLOT(logFilterFiveByFive()));


    logFilterSevenBySevenAct = new QAction(tr("7x7 LOG"), this);
    connect(logFilterSevenBySevenAct, SIGNAL(triggered()), this, SLOT(logFilterSevenBySeven()));

    generateHuffmanCodeAct = new QAction(tr("Huffman"), this);
    connect(generateHuffmanCodeAct, SIGNAL(triggered()), this, SLOT(generateHuffmanCode()));

    customFilterThreeByThreeAct = new QAction(tr("Custome Filter"), this);
    customFilterThreeByThreeAct->setShortcut(tr("Ctrl+K"));
    connect(customFilterThreeByThreeAct, SIGNAL(triggered()), this, SLOT(customFilterThreeByThree()));
}
//! [18]

//! [19]
void ImageViewer::createMenus()
//! [19] //! [20]A
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
//    viewMenu->addAction(brightnessContrastAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    logFilterMenu = new QMenu(tr("&Filters"), this);
    logFilterMenu->addAction(logFilterFiveByFiveAct);
    logFilterMenu->addAction(logFilterSevenBySevenAct);

    colourMenu = new QMenu(tr("&Colour Map"), this);
    colourMenu->addAction(redToggleAct);
    colourMenu->addAction(greenToggleAct);
    colourMenu->addAction(blueToggleAct);

    effectsMenu = new QMenu(tr("&Effects"), this);
    effectsMenu->addMenu(colourMenu);
    effectsMenu->addMenu(logFilterMenu);
    effectsMenu->addAction(resampleAct);
    effectsMenu->addAction(brightnessContrastAct);
    effectsMenu->addAction(negativeAct);
    effectsMenu->addAction(histogramsAct);
    effectsMenu->addAction(normalizationAct);
    effectsMenu->addAction(grayImageAct);
    effectsMenu->addAction(bitPlaneRunLengthCodingAct);
    effectsMenu->addAction(generateHuffmanCodeAct);
    effectsMenu->addAction(customFilterThreeByThreeAct);
    effectsMenu->setEnabled(false);


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
    effectsMenu->setEnabled(true);
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
    img->tempImage = tempImage.copy();
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
    ResamplingScaleDialog *ndlg = new ResamplingScaleDialog();
    ndlg->show();
    //ndlg->setWidth(num_of_rows);
    //ndlg->setHeight(num_of_cols);


    QImage img2((num_of_rows +1)/2 , (num_of_cols + 1)/2, QImage::Format_RGB32);

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


//    string  name = "First Last";            // NOTE: these lines of code (the variables you wish to change)
//    bool    student  = true;                //  probably exist in your program already and so it is only
//    int     age      = 20;                  //  the seven lines below needed to "create and display"
//    int     sportIdx = 1;                   //  your custom dialog.

//    CustomDialog d("Registration", this);                            // We want our custom dialog called "Registration".
//    d.addLabel    ("Please enter your details below ...");           // The first element is just a text label (non interactive).
//    d.addLineEdit ("name:  ", &name, "No middle name!");             // Here's a line edit.
//    d.addCheckBox ("current student", &student, "my tooltip");       // Here's a checkbox with a tooltip (optional last argument).
//    d.addSpinBox  ("your age: ", 1, 120, &age, 1);                   // Here's a number spin box for age.
//    d.addComboBox ("sport: ", "tennis|soccer|cricket", &sportIdx);   // And here's a combo box with three options (separated by '|').

//    d.exec();                             // Execution stops here until user closes dialog

//    if(d.wasCancelled())                // If the user hit cancel, your values stay the same
//      return;                           // and you probably don't need to do anything
//    cout << "Thanks " << name << endl;   // and here it's up to you to do stuff with your new values
//    cout << "Age " << age << endl;
}

void ImageViewer::negative(){
    int num_of_cols = tempImage.height();
    int num_of_rows = tempImage.width();
    QImage img = tempImage.copy();

    for(int row = 0 ; row < num_of_rows ; ++row){
        for (int col = 0; col < num_of_cols; ++col) {
            QRgb clr = tempImage.pixel(row,col);
            QColor pixi = QColor(clr);
            clr = qRgba(255-pixi.red(), 255-pixi.green(), 255-pixi.blue(), 255);
            img.setPixel(row,col,clr);
        }
    }

     imageLabel->setPixmap(QPixmap::fromImage(img));
     imageLabel->adjustSize();
     tempImage = img.copy();
}

void ImageViewer::brightnessContrast(){

    backupImage = tempImage.copy();

    QWidget *brightnWdgt = new QWidget();
    QWidget *contrastWdgt = new QWidget();
    QWidget *mainWdgt = new QWidget(scrollArea);

    QHBoxLayout *layout = new QHBoxLayout();
    QHBoxLayout *layout2 = new QHBoxLayout();
    QVBoxLayout *layout3 = new QVBoxLayout();

    QPushButton *btn = new QPushButton("&Ok");
    QLabel *lblBrightValue = new QLabel(QString::number(brightness));

    QPushButton *btnCancel = new QPushButton("&Cancel");
    QLabel *lblContrastValue = new QLabel(QString::number(brightness));


    QSlider *pb = new QSlider();
    pb->setMaximum(1000);
    pb->setMinimum(0);
    pb->setSliderPosition(brightness);
    pb->setOrientation(Qt::Horizontal);
//    pb->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    QSlider *pb2 = new QSlider();
    pb2->setMaximum(100);
    pb2->setMinimum(-100);
    pb2->setSliderPosition(contrast);
    pb2->setOrientation(Qt::Horizontal);
    pb2->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);



    layout->addWidget(pb);
    layout->addWidget(lblBrightValue);
    layout->addWidget(btn);

    layout2->addWidget(pb2);
    layout2->addWidget(lblContrastValue);
    layout2->addWidget(btnCancel);

    brightnWdgt->setLayout(layout);
    contrastWdgt->setLayout(layout2);

    layout3->addWidget(brightnWdgt);
    layout3->addWidget(contrastWdgt);

    mainWdgt->setLayout(layout3);
    mainWdgt->show();


    QObject::connect(pb, SIGNAL(sliderMoved(int)), this, SLOT(brightnessContrastSlot(int)));
    QObject::connect(pb2, SIGNAL(sliderMoved(int)), this, SLOT(contrastSlot(int)));
    QObject::connect(pb, SIGNAL(sliderMoved(int)), lblBrightValue, SLOT(setNum(int)));
    QObject::connect(btn, SIGNAL(clicked()), mainWdgt, SLOT(close()));
    QObject::connect(btnCancel, SIGNAL(clicked()), this, SLOT(restoreImage()));
    QObject::connect(btnCancel, SIGNAL(clicked()), mainWdgt, SLOT(close()));

}

void ImageViewer::restoreImage(){
    tempImage = backupImage.copy();
    imageLabel->setPixmap(QPixmap::fromImage(tempImage));
    imageLabel->adjustSize();
}


void ImageViewer::contrastSlot(int x){
    int num_of_cols = tempImage.height();
    int num_of_rows = tempImage.width();
    QImage img;
    if(!backupImage.isNull()){
        img = backupImage.copy();
    }else{
        img = tempImage.copy();
    }
    contrast = x;

    contrastLookupUpdate(x);


    for(int row = 0 ; row < num_of_rows ; ++row){
        for (int col = 0; col < num_of_cols; ++col) {
            QRgb clr = tempImage.pixel(row,col);
            QColor pixi = QColor(clr);
//
            clr = qRgba(contrast_lookup[pixi.red()], contrast_lookup[pixi.green()], contrast_lookup[pixi.blue()], 255);
            img.setPixel(row,col,clr);
        }

    }
 //   qDebug() << contrast_lookup[255] << 255;
  //  qDebug() << contrast_lookup[0] << 0;
   // qDebug() << contrast_lookup[150] << 150;

//    qDebug() << img.hasAlphaChannel() << endl;

     imageLabel->setPixmap(QPixmap::fromImage(img));
     imageLabel->adjustSize();
//     backupImage = img.copy();
}


void ImageViewer::contrastLookupUpdate(int x){

    double upVal = 0;
    double xx = x;
    double c = (100.0 + xx) / 100.0;
    c *= c;

    for (int i = 0; i < 256; ++i) {
        upVal = (double)i;
        upVal /= 255.0;
        upVal -=0.5;
        upVal *= c;
        upVal += 0.5;
        upVal *= 255;

        if(upVal < 0){
            upVal = 0;
        }else if(upVal > 255){
            upVal = 255;
        }

        contrast_lookup[i] = (int)upVal;
//        qDebug() << contrast_lookup[i] ;
    }

}


void ImageViewer::brightnessLookupUpdate(int x){
    float y;
    int upVal;

    for (int i = 0; i < 256; ++i) {
        y = x/(100.0);
        upVal = (int)(i*y);

        if(upVal < 0){
            upVal = 0;
        }else if(upVal > 255){
            upVal = 255;
        }

        brightness_lookup[i] = upVal;
    }

}

void ImageViewer::brightnessContrastSlot(int x){
    int num_of_cols = tempImage.height();
    int num_of_rows = tempImage.width();
    QImage img;
    if(!backupImage.isNull()){
        img = backupImage.copy();
    }else{
        img = tempImage.copy();
    }
    brightness = x;
    brightnessLookupUpdate(x);

//    float y = x/(255.0);
    for(int row = 0 ; row < num_of_rows ; ++row){
        for (int col = 0; col < num_of_cols; ++col) {
            QRgb clr = tempImage.pixel(row,col);
            QColor pixi = QColor(clr);
            clr = qRgba(brightness_lookup[pixi.red()], brightness_lookup[pixi.green()], brightness_lookup[pixi.blue()], 255);
            img.setPixel(row,col,clr);
        }
    }


     imageLabel->setPixmap(QPixmap::fromImage(img));
     imageLabel->adjustSize();
}


void ImageViewer::generateHuffmanCode(){
    updateHistograms();
    INode* root = BuildTree(red_histo);

    HuffCodeMap codes;
    GenerateCodes(root, HuffCode(), codes);
    delete root;

    for (HuffCodeMap::const_iterator it = codes.begin(); it != codes.end(); ++it)
    {
        std::cout << (int)it->first << " ";
        std::copy(it->second.begin(), it->second.end(),
                  std::ostream_iterator<bool>(std::cout));
        std::cout << std::endl;
    }
    //return 0;

}

void ImageViewer::updateHistograms(){
    int num_of_cols = tempImage.height();
    int num_of_rows = tempImage.width();
    int r,g,b;
    for(int i = 0; i < 256; ++i){
        red_histo[i] = 0;
        green_histo[i] = 0;
        blue_histo[i] = 0;
    }

    redMax = 0;
    redMin = 255;
    greenMax = 0;
    greenMin = 255;
    blueMax = 0;
    blueMin = 255;
    redMaxCount = 0;
    greenMaxCount = 0;
    blueMaxCount = 0;


    for(int row = 0 ; row < num_of_rows ; ++row){
        for (int col = 0; col < num_of_cols; ++col) {
            QRgb clr = tempImage.pixel(row,col);
            QColor pixi = QColor(clr);
            r = pixi.red();
            g = pixi.green();
            b = pixi.blue();
            red_histo[r] += 1;
            green_histo[g] += 1;
            blue_histo[b] += 1;

            if(redMaxCount < red_histo[r]){
                redMaxCount = red_histo[r];
            }
            if(greenMaxCount < green_histo[g]){
                greenMaxCount = green_histo[g];
            }

            if(blueMaxCount < blue_histo[b]){
                blueMaxCount = blue_histo[b];
            }

            if(r > redMax){
                redMax = r;
            }
            if( r < redMin ){
                redMin = r;
            }

            if(g > greenMax){
                greenMax = g;
            }
            if( g < greenMin ){
                greenMin = g;
            }

            if(b > blueMax){
                blueMax = b;
            }
            if(b < blueMin ){
                blueMin = b;
            }

        }
    }
}

void ImageViewer::showHistograms(){
    updateHistograms();
    int redMax, greenMax, blueMax;
    QCustomPlot *customPlot = new QCustomPlot();
    QCustomPlot *customPlot2 = new QCustomPlot();
    QCustomPlot *customPlot3 = new QCustomPlot();
    redMax = redMaxCount;
    greenMax = greenMaxCount;
    blueMax = blueMaxCount;

    QVector<double> x(256), r(256), g(256), b(256); // initialize with entries 0..100
    for (int i=0; i<256; ++i)
    {
      x[i] = i; // x goes from 0 to 255
      r[i] = red_histo[i];
      g[i] = green_histo[i];
      b[i] = blue_histo[i];

    }

    customPlot->xAxis->setLabel("Red Intensity");
    customPlot->yAxis->setLabel("Pixel Count");
    customPlot->xAxis->setRange(0, 256);
    customPlot->yAxis->setRange(0, redMax +10);

    customPlot2->xAxis->setLabel("Green Intensity");
    customPlot2->yAxis->setLabel("Pixel Count");
    customPlot2->xAxis->setRange(0, 256);
    customPlot2->yAxis->setRange(0, greenMax +10);

    customPlot3->xAxis->setLabel("Blue Intensity");
    customPlot3->yAxis->setLabel("Pixel Count");
    customPlot3->xAxis->setRange(0, 256);
    customPlot3->yAxis->setRange(0, blueMax +10);

    QCPBars *bars1 = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(bars1);
    bars1->setWidth((9/(double)x.size())+1);
    bars1->setPen(Qt::NoPen);
    bars1->setBrush(QColor(255, 0, 0, 255));
    bars1->setData(x, r);

    QCPBars *greenBar = new QCPBars(customPlot2->xAxis, customPlot2->yAxis);
    customPlot2->addPlottable(greenBar);
    greenBar->setWidth((9/(double)x.size())+1);
    greenBar->setPen(Qt::NoPen);
    greenBar->setBrush(QColor(0, 255, 0, 255));
    greenBar->setData(x, g);

    QCPBars *blueBar = new QCPBars(customPlot3->xAxis, customPlot3->yAxis);
    customPlot3->addPlottable(blueBar);
    blueBar->setWidth((9/(double)x.size())+1);
    blueBar->setPen(Qt::NoPen);
    blueBar->setBrush(QColor(0, 0, 255, 255));
    blueBar->setData(x, g);


    customPlot->replot();
    customPlot->setViewport(QRect(10,10,20,20));
    customPlot->setWindowTitle("Red histogram");
    customPlot->show();
    customPlot->resize(300,200);


    customPlot2->replot();
    customPlot2->setViewport(QRect(50,30,50,60));
    customPlot2->setWindowTitle("Green histogram");
    customPlot2->show();
    customPlot2->resize(300,200);

    customPlot3->replot();
    customPlot->setWindowOpacity(50);
    customPlot3->setViewport(QRect(50,30,20,20));
    customPlot3->setWindowTitle("Blue histogram");
    customPlot3->show();
    customPlot3->resize(300,200);

}

void ImageViewer::normalize(int min, int max){
    updateHistograms();
    int num_of_cols = tempImage.height();
    int num_of_rows = tempImage.width();
    QImage img = tempImage.copy();
    int ro, go, bo, ra, ga, ba;


    for(int row = 0 ; row < num_of_rows ; ++row){
        for (int col = 0; col < num_of_cols; ++col) {
            QRgb clr = tempImage.pixel(row,col);
            QColor pixi = QColor(clr);
            ro = pixi.red();
            go = pixi.green();
            bo = pixi.blue();
            ra = (ro - redMin)*((max-min))/(redMax-redMin)*1.0 + min;
            ga = (go -greenMin)*((max-min))/(greenMax-greenMin)*1.0 + min;
            ba = (bo - blueMin)*((max-min))/(blueMax-blueMin)*1.0 + min;

            clr = qRgba(ra, ga, ba, 255);
            img.setPixel(row,col,clr);
        }
    }

     imageLabel->setPixmap(QPixmap::fromImage(img));
     imageLabel->adjustSize();
     tempImage = img.copy();
}

void ImageViewer::grayImage(){
    int num_of_cols = tempImage.height();
    int num_of_rows = tempImage.width();
    int ro, go, bo, y;
    char x;
    QImage image(num_of_rows, num_of_cols, QImage::Format_Grayscale8);
    //QRgb value;

    for(int row = 0 ; row < num_of_rows ; ++row){
        for (int col = 0; col < num_of_cols; ++col) {
            QRgb clr = tempImage.pixel(row,col);
            QColor pixi = QColor(clr);
            ro = pixi.red();
            go = pixi.green();
            bo = pixi.blue();

            //Y = 0.299R + 0.587G + 0.114B
            y = (int)(0.299*ro + 0.587*go + 0.114*bo);
            x = (char)y;
            //qDebug() << y;
            image.setPixel(row, col,x);
        }
    }

     imageLabel->setPixmap(QPixmap::fromImage(image));
     imageLabel->adjustSize();
     tempImage = image.copy();
     scaleImage(1);
}

void ImageViewer::logFilterSevenBySeven(){
    /*
        -5   -6    -5.5   -5 -5.5   -6    -5
        -6   -5    -2    0.8   -2   -5    -6
        -5.5 -2    0.4   0.4  0.4   -2  -5.5
        -5    0.8  0.4   225  0.4  0.8   -5
        -5.5 -2    0.4   0.4  0.4  -2   -5.5
        -6   -5     -2   0.8   -2  -5    -6,
        -5   -6   -5.5    -5 -5.5  -6    -5
      */
    int num_of_cols = tempImage.height();
    int num_of_rows = tempImage.width();

    float mask7b7[7][7] = {
        {-5,-6,-5.5,-5,-5.5,-6,-5},
        {-6,-5,-2,0.8,-2,-5,-6},
        {-5.5,-2,0.4,0.4,0.4,-2,-5.5},
        {-5,0.8,0.4,225,0.4,0.8,-5},
        {-5.5,-2,0.4,0.4,0.4,-2,-5.5},
        {-6,-5,-2,0.8,-2,-5,-6},
        {-5,-6,-5.5,-5,-5.5,-6,-5},
    };


    QImage image = tempImage.copy();
    int planes = tempImage.depth();

    for(int row = 3 ; row < num_of_rows-3 ; ++row){
        for (int col = 3; col < num_of_cols-3; ++col) {
            double ro = 0, go = 0, bo = 0;
            int rst = row - 3;
            int cst = col - 3;
            for( int r = rst; r < row + 4; r++){
                for( int c = cst; c < col + 4; c++){
                    QRgb clr = tempImage.pixel(r, c);
                    QColor pixi = QColor(clr);
                    int x = r - rst;
                    int y = c - cst;
                    ro += pixi.red()*mask7b7[x][y];
                    go += pixi.green()*mask7b7[x][y];
                    bo += pixi.blue()*mask7b7[x][y];
                }
            }



            QRgb clrLOG;
            if(planes == 8){
                int y = (int)(0.299*ro + 0.587*go + 0.114*bo);
                char x = (char)y;
                if(x > 256) x = 256;
                if(x < 0) x = 0;
                image.setPixel(row, col, x);
            }else{
                int a, b, c;
                a = qCeil(ro);
                b = qCeil(go);
                c = qCeil(bo);
                if(a > 256) a = 256;
                if(a < 0) a = 0;
                if(b > 256) b = 256;
                if(b < 0) b = 0;
                if(c > 256) c = 256;
                if(c < 0) c = 0;

                clrLOG = qRgba(a, b, c, 255);
                 image.setPixel(row, col, clrLOG);
            }

        }
    }

     imageLabel->setPixmap(QPixmap::fromImage(image));
     imageLabel->adjustSize();
     tempImage = image.copy();
     scaleImage(1);
}


void ImageViewer::cFThreeByThree(int mask[][3]){


    for(int c = 0; c < 3 ; c++){
        for(int r = 0; r < 3; r++){
            qDebug() << mask[c][r] << " " ;
        }
        qDebug() << endl;
    }



    int num_of_cols = tempImage.height();
    int num_of_rows = tempImage.width();


    QImage image = tempImage.copy();
    int planes = tempImage.depth();

    for(int row = 1 ; row < num_of_rows-1 ; ++row){
        for (int col = 1; col < num_of_cols-1; ++col) {
             double ro = 0, go = 0, bo = 0;
            int rst = row - 1;
            int cst = col - 1;
            for( int r = rst; r < row + 2; r++){
                for( int c = cst; c < col + 2; c++){
                    QRgb clr = tempImage.pixel(r, c);
                    QColor pixi = QColor(clr);
                    int x = r - rst;
                    int y = c - cst;
                    ro += pixi.red()*mask[y][x];
                    go += pixi.green()*mask[y][x];
                    bo += pixi.blue()*mask[y][x];
                }
            }



            QRgb clrLOG;
            if(planes == 8){
                int y = (int)(0.299*ro + 0.587*go + 0.114*bo);
                char x = (char)y;
                if(x > 256) x = 256;
                if(x < 0) x = 0;
                image.setPixel(row, col, x);
            }else{
                int a, b, c;
                a = qCeil(ro);
                b = qCeil(go);
                c = qCeil(bo);
                if(a > 256) a = 256;
                if(a < 0) a = 0;
                if(b > 256) b = 256;
                if(b < 0) b = 0;
                if(c > 256) c = 256;
                if(c < 0) c = 0;

                clrLOG = qRgba(a, b, c, 255);
                 image.setPixel(row, col, clrLOG);
            }

        }
    }

     imageLabel->setPixmap(QPixmap::fromImage(image));
     imageLabel->adjustSize();
     tempImage = image.copy();
     scaleImage(1);

}

void ImageViewer::customFilterThreeByThree(){
    /*
             x  x   x
             x  x   x
             x  x   x
      */
    CustomFilter *cd = new CustomFilter(*this, "3x3 Filter", &ImageViewer::cFThreeByThree);
    cd->exec();
    delete cd;
}


void ImageViewer::logFilterFiveByFive(){
    /*
             0   0  -1   0   0
             0  -1  -2  -1   0
            -1  -2  16  -2  -1
             0  -1  -2  -1   0
             0   0  -1   0   0
      */

    int num_of_cols = tempImage.height();
    int num_of_rows = tempImage.width();

    QImage image = tempImage.copy();
    int planes = tempImage.depth();

    for(int row = 2 ; row < num_of_rows-2 ; ++row){
        for (int col = 2; col < num_of_cols-2; ++col) {
            int ro, go, bo;
            QRgb clr = tempImage.pixel(row,col);
                QColor pixi = QColor(clr);
//                ro += 256;
//                go += 256;
//                bo += 256;
                ro = pixi.red()*16;
                go = pixi.green()*16;
                bo = pixi.blue()*16;

            // N4 Clan
            QRgb cll2 = tempImage.pixel(row-1,col);
                QColor cll2p = QColor(cll2);
                ro -= cll2p.red()*2;
                go -= cll2p.green()*2;
                bo -= cll2p.blue()*2;
            QRgb clr2 = tempImage.pixel(row,col-1);
                QColor clr2p = QColor(clr2);
                ro -= clr2p.red()*2;
                go -= clr2p.green()*2;
                bo -= clr2p.blue()*2;
            QRgb clu2 = tempImage.pixel(row,col+1);
                QColor clu2p = QColor(clu2);
                ro -= clu2p.red()*2;
                go -= clu2p.green()*2;
                bo -= clu2p.blue()*2;
            QRgb cld2 = tempImage.pixel(row+1,col);
                QColor cld2p = QColor(cld2);
                ro -= cld2p.red()*2;
                go -= cld2p.green()*2;
                bo -= cld2p.blue()*2;

            // -1 clan
            clr = tempImage.pixel(row-1,col-1);
                pixi = QColor(clr);
                ro -= pixi.red();
                go -= pixi.green();
                bo -= pixi.blue();
            clr = tempImage.pixel(row-1,col+1);
                pixi = QColor(clr);
                ro -= pixi.red();
                go -= pixi.green();
                bo -= pixi.blue();
            clr = tempImage.pixel(row+1,col+1);
                pixi = QColor(clr);
                ro -= pixi.red();
                go -= pixi.green();
                bo -= pixi.blue();
            clr = tempImage.pixel(row+1,col-1);
                pixi = QColor(clr);
                ro -= pixi.red();
                go -= pixi.green();
                bo -= pixi.blue();

            clr = tempImage.pixel(row,col-2);
                pixi = QColor(clr);
                ro -= pixi.red();
                go -= pixi.green();
                bo -= pixi.blue();
            clr = tempImage.pixel(row,col+2);
                pixi = QColor(clr);
                ro -= pixi.red();
                go -= pixi.green();
                bo -= pixi.blue();
            clr = tempImage.pixel(row-2,col);
                pixi = QColor(clr);
                ro -= pixi.red();
                go -= pixi.green();
                bo -= pixi.blue();
            clr = tempImage.pixel(row+2,col);
                pixi = QColor(clr);
                ro -= pixi.red();
                go -= pixi.green();
                bo -= pixi.blue();

                QRgb clrLOG;

//                ro -= 256;
//                go -= 256;
//                bo -= 256;

            if(planes == 8){
                int y = (int)(0.299*ro + 0.587*go + 0.114*bo);
                char x = (char)y;
                image.setPixel(row, col, x);
            }else{
                clrLOG = qRgba(ro, go, bo, 255);
                 image.setPixel(row, col, clrLOG);
            }

        }
    }

     imageLabel->setPixmap(QPixmap::fromImage(image));
     imageLabel->adjustSize();
     tempImage = image.copy();
     scaleImage(1);
}

void ImageViewer::bitPlaneRunLengthCoding(){
    int num_of_cols = tempImage.height();
    int num_of_rows = tempImage.width();
    int planes;
    int r, g, b;

    QImage img = tempImage.copy();
    planes = img.depth();

    QVector<QBitArray> vectorPlanes;

    for(int row = 0 ; row < num_of_rows ; ++row){
        for (int col = 0; col < num_of_cols; ++col) {
            QRgb clr = tempImage.pixel(row,col);
            QColor pixi = QColor(clr);
            QBitArray bitPlanes(planes);

            if(planes == 32){
                r = pixi.red();
                g =  pixi.green();
                b =  pixi.blue();
                for (int i = 0; i < 8; ++i) {
                    bitPlanes[i]    = r && 2^(8-i);
                    bitPlanes[i+8]  = g && 2^(8-i);
                    bitPlanes[i+16] = b && 2^(8-i);
                }
            }else if(planes == 8){
                for (int i = 0; i < 8; ++i) {
                    bitPlanes[i]    = pixi.value() && 2^(8-i);
                }
            }
            vectorPlanes.append(bitPlanes);
        }
    }

    if(planes == 32) planes = planes - 8;

    QVector< QVector<int> > bitpl;
    bitpl.resize(planes);

    for(int i = 0 ; i < planes; i++){
        int prev = vectorPlanes[0].at(i);
//        qDebug() << prev << " " << vectorPlanes[0].at(i);
        int curr = 0;
        int count = 1;
        for(int pl = 1; pl < num_of_cols*num_of_rows; pl++){
            curr = vectorPlanes[pl].at(i);
            if(prev == curr){
                count++;
            }else{
                bitpl[i].append(prev);
                bitpl[i].append(count);
                count = 1;
            }
            prev = curr;
        }

    }


        QString filename = openedFileName;
        filename.chop(4);
        filename.append(".txt");

        QFile file(filename);
        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            for(int i = 0; i < planes; i++){
                stream << "Plane " << i+1 << endl;
                while(!bitpl[i].isEmpty()){
                    stream << bitpl[i].takeLast() << "-" << bitpl[i].takeLast();
                    stream << " ";
                }
                stream << endl;
            }
        }
        file.close();
}


void ImageViewer::normalization(){
    ContrastStretchingDialog *ad = new ContrastStretchingDialog(this);
    QObject::connect(ad, SIGNAL(valuesUpdated(int,int)), this, SLOT(normalize(int,int)));
    ad->show();
}
