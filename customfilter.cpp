#include "customfilter.h"
#include "ui_customfilter.h"

CustomFilter::CustomFilter(ImageViewer &parent, QString title, _funcType func) :

    ui(new Ui::CustomFilter),
    m_func(func),
    parent_window(parent)
{
    ui->setupUi(this);
    this->setWindowTitle(title);
}

CustomFilter::~CustomFilter()
{
    delete ui;
}

void CustomFilter::on_buttonBox_accepted()
{

    int arr[3][3] = {};
    arr[0][0] = (int)ui->textEdit_1->toPlainText().toInt();
    arr[0][1] = (int)ui->textEdit_2->toPlainText().toInt();
    arr[0][2] = (int)ui->textEdit_3->toPlainText().toInt();

    arr[1][0] = (int)ui->textEdit_4->toPlainText().toInt();
    arr[1][1] = (int)ui->textEdit_5->toPlainText().toInt();
    arr[1][2] = (int)ui->textEdit_6->toPlainText().toInt();

    arr[2][0] = (int)ui->textEdit_7->toPlainText().toInt();
    arr[2][1] = (int)ui->textEdit_8->toPlainText().toInt();
    arr[2][2] = (int)ui->textEdit_9->toPlainText().toInt();
    ((parent_window).*m_func)(arr);
}
