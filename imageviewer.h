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

#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#endif



QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
QT_END_NAMESPACE

//! [0]
class ImageViewer : public QMainWindow
{
    Q_OBJECT

public:
    ImageViewer();

    bool loadFile(const QString &);

private slots:
    void open();
    void saveas();
    void cloneWindow();
    void print();
    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();
    void about();
    void redToggle();
    void greenToggle();
    void blueToggle();
    void imageResample();
    void negative();
    void brightnessContrast();
    void brightnessContrastSlot(int x);
    void contrastSlot(int x);
    void restoreImage();
    void showHistograms();
    void normalization();
    void normalize(int min, int max);
    void grayImage();
    void bitPlaneRunLengthCoding();
    void logFilterFiveByFive();
    void logFilterSevenBySeven();
    void customFilterThreeByThree();
    void generateHuffmanCode();

private:
    void createActions();
    bool saveFile(const QString &);
    void createMenus();
    void updateActions();
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);
    void loadImage(QImage);    
    void nearestNeighbourResample();
    void contrastLookupUpdate(int x);
    void brightnessLookupUpdate(int x);
    void updateHistograms();
    void cFThreeByThree(int mask[3][3]);




    QLabel *imageLabel;
    QScrollArea *scrollArea;
//    ImageProcessor myImg;


    double scaleFactor;
    QImage tempImage;
    QImage backupImage;
    QString openedFileName;
    bool greenOn;
    bool blueOn;
    bool redOn;
    int brightness;
    int contrast;
    int contrast_lookup[256];
    int brightness_lookup[256];
    int red_histo[256];
    int green_histo[256];
    int blue_histo[256];

    int redMax;
    int redMaxCount;
    int redMin;

    int greenMax;
    int greenMaxCount;
    int greenMin;

    int blueMax;
    int blueMaxCount;
    int blueMin;

#ifndef QT_NO_PRINTER
    QPrinter printer;
#endif

    QAction *openAct;
    QAction *printAct;
    QAction *cloneWindowAct;
    QAction *exitAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

    QAction *saveasAct;
    QAction *resampleAct;
    QAction *greenToggleAct;
    QAction *redToggleAct;
    QAction *blueToggleAct;
    QAction *negativeAct;
    QAction *brightnessContrastAct;
    QAction *histogramsAct;
    QAction *normalizationAct;
    QAction *grayImageAct;
    QAction *bitPlaneRunLengthCodingAct;
    QAction *logFilterFiveByFiveAct;
    QAction *logFilterSevenBySevenAct;
    QAction *generateHuffmanCodeAct;
    QAction *customFilterThreeByThreeAct;

    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QMenu *effectsMenu;
    QMenu *colourMenu;
    QMenu *logFilterMenu;
};
//! [0]

#endif
