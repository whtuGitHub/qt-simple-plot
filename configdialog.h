#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QColorDialog>

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();

    void setColor(QColor color);
    void setName(QString name);
    QColor color();
    QString name();

private slots:
    void on_pushButton_clicked();

    void on_colorDialog_accepted();

private:
    Ui::ConfigDialog *ui;
    QColorDialog colorDialog;
    QColor currentColor;
};

#endif // CONFIGDIALOG_H
