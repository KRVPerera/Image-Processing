cmake_minimum_required(VERSION 3.21)
project(Image_Processing)

set(QT_VERSION_REQ "5.2")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        PrintSupport
        Xml
        Quick
        REQUIRED)

QT5_WRAP_UI( UI_HDRS
        contraststretchingdialog.ui
        customfilter.ui
        resamplingscaledialog.ui
        )

add_executable(Image_Processing
        catch.hpp
        contraststretchingdialog.cpp
        customdialog.cpp
        customfilter.cpp
        huffman.cpp
        imageviewer.cpp
        main.cpp
        qcustomplot.cpp
        resamplingscaledialog.cpp
        ${UI_HDRS}
        )

target_link_libraries(Image_Processing
        Qt5::Core
        Qt5::Widgets
        Qt5::PrintSupport
        Qt5::Xml
        Qt5::Quick
        Qt5::Gui
        )

