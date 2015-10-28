#ifndef CUSTOMFILTER_H
#define CUSTOMFILTER_H

#include <QDialog>

namespace Ui {
class CustomFilter;
}

class CustomFilter : public QDialog
{
    Q_OBJECT

public:
    explicit CustomFilter(QWidget *parent = 0);
    ~CustomFilter();

private:
    Ui::CustomFilter *ui;
};

#endif // CUSTOMFILTER_H
