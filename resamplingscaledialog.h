#ifndef RESAMPLINGSCALEDIALOG_H
#define RESAMPLINGSCALEDIALOG_H

#include <QDialog>

namespace Ui {
class ResamplingScaleDialog;
}

class ResamplingScaleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResamplingScaleDialog(QWidget *parent = 0);
    ~ResamplingScaleDialog();

private:
    Ui::ResamplingScaleDialog *ui;
};

#endif // RESAMPLINGSCALEDIALOG_H
