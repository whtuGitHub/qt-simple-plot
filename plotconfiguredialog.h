#ifndef PLOTCONFIGUREDIALOG_H
#define PLOTCONFIGUREDIALOG_H

#include <QDialog>

namespace Ui {
class PlotConfigureDialog;
}

class PlotConfigureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlotConfigureDialog(QWidget *parent = 0);
    ~PlotConfigureDialog();

    void setXLabel(QString label);
    void setYLabel(QString label);

    QString xLabel();
    QString yLabel();

private:
    Ui::PlotConfigureDialog *ui;
};

#endif // PLOTCONFIGUREDIALOG_H
