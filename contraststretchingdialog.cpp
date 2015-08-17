#include "contraststretchingdialog.h"
#include "ui_contraststretchingdialog.h"
#include <QDebug>

ContrastStretchingDialog::ContrastStretchingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContrastStretchingDialog)
{
    ui->setupUi(this);
    QObject::connect(this, SIGNAL(valuesUpdated(int,int)), this, SLOT(print_minmax(int,int)));
}

ContrastStretchingDialog::~ContrastStretchingDialog()
{
    delete ui;
}

void ContrastStretchingDialog::on_buttonBox_accepted()
{
    int max,min;
    max =  ui->spbox_max->value();
    min = ui->spbox_min->value();
    emit valuesUpdated(min,max);
}

void ContrastStretchingDialog::print_minmax(int min, int max){
    qDebug() << min << max ;//<< endl;
}
