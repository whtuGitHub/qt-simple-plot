#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (qApp->arguments().size() > 1)
    {
        dataSeries.loadFromFile(qApp->arguments().at(1));

        for (int i=0; i<dataSeries.size(); i++)
        {
            QPixmap pixmap(20, 20);
            pixmap.fill(dataSeries[i]->color);
            QIcon icon(pixmap);
            ui->seriesCombo->addItem(icon, dataSeries[i]->name, i);
        }
    }
    else
        qApp->quit(); //display error

    connect(&area, SIGNAL(measurePointAdded(int,int)), this, SLOT(on_measurePointAdded(int,int)));
    connect(&configDialog, SIGNAL(accepted()), this, SLOT(on_configDialogAccepted()));
    connect(&plotConfigureDialog, SIGNAL(accepted()),
            this, SLOT(on_plotConfigureDialogAccepted()));
    connect(ui->selectedPoints->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(on_selectedPointsActivated(QModelIndex,QModelIndex)));

    area.setDataSeries(&dataSeries);
    ui->gridLayout->replaceWidget(ui->placeHolder, &area);
    area.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    ui->actionX_zoom->setChecked(settings.value("zoomX", true).toBool());
    ui->actionY_zoom->setChecked(settings.value("zoomY", true).toBool());
    ui->actionPan->setChecked(settings.value("pan", true).toBool());
    ui->actionMeasure->setChecked(settings.value("measure", false).toBool());

    resize(1024,500);

    xLabel = "x axis";
    yLabel = "y axis";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_measurePointAdded(int series, int pointIndex)
{
    if (dataSeries[series]->selectedPoints.rowCount() < 2)
    {
        QStandardItem* point = new QStandardItem();
        point->setData(pointIndex);
        point->setText(QString::number(dataSeries[series]->at(pointIndex).x()) + QString(", ") +
                       QString::number(dataSeries[series]->at(pointIndex).y()));
        dataSeries[series]->selectedPoints.appendRow(point);
        area.update();
    }
    updateMeasures();
}

void MainWindow::on_actionY_zoom_toggled(bool toggled)
{
    settings.setValue("zoomY", toggled);
}

void MainWindow::on_actionX_zoom_toggled(bool toggled)
{
    settings.setValue("zoomX", toggled);
}

void MainWindow::on_actionPan_toggled(bool toggled)
{
    if (toggled)
    {
        ui->actionMeasure->setChecked(false);
        area.setSelectedPoint(-1);
        area.update();
    }
    settings.setValue("pan", toggled);

    area.setCursor(QCursor(Qt::OpenHandCursor));
}

void MainWindow::on_actionMeasure_toggled(bool toggled)
{
    if (toggled)
        ui->actionPan->setChecked(false);
    settings.setValue("measure", toggled);

    area.setMouseTracking(toggled);
    area.setActiveSeries(ui->seriesCombo->currentIndex());

    area.setCursor(QCursor(Qt::ArrowCursor));
}

void MainWindow::on_seriesCombo_currentIndexChanged(int index)
{
    area.setActiveSeries(index);
    ui->selectedPoints->setModel(&dataSeries[index]->selectedPoints);
}

void MainWindow::on_deleteButton_clicked()
{
    foreach (QModelIndex index, ui->selectedPoints->selectionModel()->selectedIndexes())
    {
        ui->selectedPoints->model()->removeRow(index.row());
    }
    updateMeasures();
    area.setSelectedPoint(-1);
    area.update();
}

void MainWindow::updateMeasures()
{
    if (dataSeries[ui->seriesCombo->currentIndex()]->selectedPoints.rowCount() == 2)
    {
        DataArray* array = dataSeries[ui->seriesCombo->currentIndex()];
        QPointF p1, p2;
        int i1 = array->selectedPoints.item(0,0)->data().toInt(),
                i2 = array->selectedPoints.item(1,0)->data().toInt();

        if (i1 > i2)
        {
            int tmp = i1;
            i1 = i2;
            i2 = tmp;
        }

        p1 = array->at(i1);
        p2 = array->at(i2);

        float min, max;
        array->yMinMax(min, max, i1, i2);

        QString text("dy/dx:\n");
        text += QString::number((p2.y()-p1.y())/(p2.x()-p1.x()));
        text += QString("\nMin:\n") += QString::number(min);
        text += QString("\nMax:\n") += QString::number(max);
        ui->measures->setText(text);
    }
    else
        ui->measures->setText("");
}

void MainWindow::on_configureButton_clicked()
{
    DataArray* array = dataSeries[ui->seriesCombo->currentIndex()];
    configDialog.setColor(array->color);
    configDialog.setName(array->name);
    configDialog.show();
}

void MainWindow::on_configDialogAccepted()
{
    int index = ui->seriesCombo->currentIndex();
    DataArray* array = dataSeries[index];

    array->name = configDialog.name();
    array->color = configDialog.color();

    ui->seriesCombo->setItemText(index, array->name);

    QPixmap pixmap(20, 20);
    pixmap.fill(array->color);
    QIcon icon(pixmap);
    ui->seriesCombo->setItemIcon(index, icon);
}

void MainWindow::on_selectedPointsActivated(QModelIndex index, QModelIndex previous)
{
    DataArray* array = dataSeries[ui->seriesCombo->currentIndex()];
    area.setSelectedPoint(array->selectedPoints.item(index.row(), 0)->data().toInt());
    area.update();
}

void MainWindow::on_actionConfigure_triggered()
{
    plotConfigureDialog.setXLabel(xLabel);
    plotConfigureDialog.setYLabel(yLabel);
    plotConfigureDialog.show();
}

void MainWindow::on_plotConfigureDialogAccepted()
{
    xLabel = plotConfigureDialog.xLabel();
    yLabel = plotConfigureDialog.yLabel();
    ui->xAxisLabel->setText(xLabel);
    ui->yAxisLabel->setText(yLabel);
}
