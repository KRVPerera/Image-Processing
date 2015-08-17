#include "resamplingscaledialog.h"
#include "ui_resamplingscaledialog.h"

ResamplingScaleDialog::ResamplingScaleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResamplingScaleDialog)
{
    ui->setupUi(this);
}

ResamplingScaleDialog::~ResamplingScaleDialog()
{
    delete ui;
}
