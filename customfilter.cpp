#include "customfilter.h"
#include "ui_customfilter.h"

CustomFilter::CustomFilter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomFilter)
{
    ui->setupUi(this);
}

CustomFilter::~CustomFilter()
{
    delete ui;
}
