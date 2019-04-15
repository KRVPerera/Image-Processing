#ifndef CUSTOMFILTER_H
#define CUSTOMFILTER_H

#include <QDialog>
#include <imageviewer.h>

namespace Ui {
class CustomFilter;
}

class CustomFilter : public QDialog
{
    Q_OBJECT

public:
    typedef void (ImageViewer::*_funcType)(int [][3]);
    _funcType m_func;
     ImageViewer &parent_window;
    explicit CustomFilter(ImageViewer &parent, QString title, _funcType func) ;
    ~CustomFilter();

private slots:
     void on_buttonBox_accepted();

private:
    Ui::CustomFilter *ui;
};

#endif // CUSTOMFILTER_H
