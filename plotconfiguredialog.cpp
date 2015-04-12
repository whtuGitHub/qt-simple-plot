#include "plotconfiguredialog.h"
#include "ui_plotconfiguredialog.h"

PlotConfigureDialog::PlotConfigureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotConfigureDialog)
{
    ui->setupUi(this);
}

PlotConfigureDialog::~PlotConfigureDialog()
{
    delete ui;
}

void PlotConfigureDialog::setXLabel(QString label)
{
    ui->xAxisLine->setText(label);
}

void PlotConfigureDialog::setYLabel(QString label)
{
    ui->yAxisLine->setText(label);
}

QString PlotConfigureDialog::xLabel()
{
    return ui->xAxisLine->text();
}

QString PlotConfigureDialog::yLabel()
{
    return ui->yAxisLine->text();
}
