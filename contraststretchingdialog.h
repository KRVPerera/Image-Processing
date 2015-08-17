#ifndef CONTRASTSTRETCHINGDIALOG_H
#define CONTRASTSTRETCHINGDIALOG_H

#include <QDialog>

namespace Ui {
class ContrastStretchingDialog;
}

class ContrastStretchingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ContrastStretchingDialog(QWidget *parent = 0);
    ~ContrastStretchingDialog();

private slots:
    void on_buttonBox_accepted();
    void print_minmax(int min, int max);

private:
    Ui::ContrastStretchingDialog *ui;

signals:
    void valuesUpdated(int min, int max);
};

#endif // CONTRASTSTRETCHINGDIALOG_H
