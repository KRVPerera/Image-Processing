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

#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>

#include "imageviewer.h"
//#include "pixel.h"
#include "contraststretchingdialog.h"
#include "normalizesizingdialog.h"

//#include "imageprocessor.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QGuiApplication::setApplicationDisplayName(ImageViewer::tr("KRV's Image Viewer"));
//    QCommandLineParser commandLineParser;
//    commandLineParser.addHelpOption();
//    commandLineParser.addPositionalArgument(ImageViewer::tr("[file]"), ImageViewer::tr("Image file to open."));
//    commandLineParser.process(QCoreApplication::arguments());
    ImageViewer imageViewer;
//    ImageProcessor image;
//    if (!commandLineParser.positionalArguments().isEmpty()
//        && !imageViewer.loadFile(commandLineParser.positionalArguments().front())) {
//        return -1;
//    }



    imageViewer.loadFile("/home/drex/Pictures/1.jpg");
    imageViewer.show();

//    NormalizeSizingDialog *pp = new NormalizeSizingDialog();
//    pp->show();

//    Pixel pix(10,20,30);
//    qDebug() << pix.getBlue() ;
//    ImageProcessor imp;
//    imp.setHeight(10);
//    imp.setWidth(10);
//    qDebug() << imp.getN4neighbours(9,5);

    return app.exec();
}
