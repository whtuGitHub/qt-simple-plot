#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <drawingarea.h>
#include <configdialog.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void plot();

private slots:
    void on_measurePointAdded(int series, int pointIndex);

    void on_actionY_zoom_toggled(bool toggled);

    void on_actionX_zoom_toggled(bool toggled);

    void on_actionPan_toggled(bool toggled);

    void on_actionMeasure_toggled(bool toggled);

    void on_seriesCombo_currentIndexChanged(int index);

    void on_deleteButton_clicked();

    void on_configureButton_clicked();

    void on_configureDialog_accepted();

private:
    Ui::MainWindow *ui;
    QSettings settings;
    DataSeries dataSeries;
    DrawingArea area;
    ConfigDialog configDialog;

    void updateMeasures();
};

#endif // MAINWINDOW_H