#include "configdialog.h"
#include "ui_configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);

    connect(&colorDialog, SIGNAL(accepted()), this, SLOT(on_colorDialog_accepted()));
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::setColor(QColor color)
{
    currentColor = color;
    colorDialog.setCurrentColor(color);

    QPixmap pixmap(20, 20);
    pixmap.fill(color);
    QIcon icon(pixmap);
    ui->pushButton->setIcon(icon);
}

void ConfigDialog::setName(QString name)
{
    ui->lineEdit->setText(name);
}

QColor ConfigDialog::color()
{
    return currentColor;
}

QString ConfigDialog::name()
{
    return ui->lineEdit->text();
}

void ConfigDialog::on_pushButton_clicked()
{
    colorDialog.show();
}

void ConfigDialog::on_colorDialog_accepted()
{
    setColor(colorDialog.currentColor());
}
