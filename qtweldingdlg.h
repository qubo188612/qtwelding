#ifndef QTWELDINGDLG_H
#define QTWELDINGDLG_H

#include <QDialog>

namespace Ui {
class qtweldingDlg;
}

class qtweldingDlg : public QDialog
{
    Q_OBJECT

public:
    explicit qtweldingDlg(QWidget *parent = nullptr);
    ~qtweldingDlg();

private:
    Ui::qtweldingDlg *ui;
};

#endif // QTWELDINGDLG_H
