/*  Copyright (C) 2015 Marco Scarpetta
 *
 *     This file is part of qt-simple-plot.
 *
 *  qt-simple-plot is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  qt-simple-plot is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with qt-simple-plot.  If not, see <http://www.gnu.org/licenses/>
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QXmlStreamWriter>
#include <QtSvg/QSvgGenerator>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    xLabel = "x axis";
    yLabel = "y axis";

    area.setXAxisLabel(xLabel);
    area.setYAxisLabel(yLabel);

    if (qApp->arguments().size() > 1)
    {
        loadFile(qApp->arguments().at(1));
    }
    else
        area.setDataSeries(0);

    connect(&area, SIGNAL(measurePointAdded(int,int)), this, SLOT(on_measurePointAdded(int,int)));
    connect(&configDialog, SIGNAL(accepted()), this, SLOT(on_configDialogAccepted()));
    connect(&plotConfigureDialog, SIGNAL(accepted()),
            this, SLOT(on_plotConfigureDialogAccepted()));
    connect(&fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(on_fileDialogAccepted(QString)));

    ui->gridLayout->replaceWidget(ui->placeHolder, &area);
    area.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    ui->actionX_zoom->setChecked(settings.value("zoomX", true).toBool());
    ui->actionY_zoom->setChecked(settings.value("zoomY", true).toBool());
    ui->actionPan->setChecked(settings.value("pan", true).toBool());
    ui->actionMeasure->setChecked(settings.value("measure", false).toBool());

    resize(1024,500);

    exporting = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadFile(QString fileName)
{
    while (ui->seriesCombo->count())
        ui->seriesCombo->removeItem(0);

    savedFile = fileName;
    if (!restore(fileName))
    {
        savedFile = "";
        inputFile = fileName;
        dataSeries.loadFromFile(inputFile);

        for (int i=0; i<dataSeries.size(); i++)
        {
            QPixmap pixmap(20, 20);
            pixmap.fill(dataSeries[i]->color);
            QIcon icon(pixmap);
            ui->seriesCombo->addItem(icon, dataSeries[i]->name, i);
        }
        area.setDataSeries(&dataSeries);
    }
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
    if (index > -1)
    {
        area.setActiveSeries(index);
        ui->selectedPoints->setModel(&dataSeries[index]->selectedPoints);

        connect(ui->selectedPoints->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
                this, SLOT(on_selectedPointsActivated(QModelIndex,QModelIndex)));

        updateMeasures();
    }
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

        qreal min, max;
        array->yMinMax(min, max, i1, i2);

        QString text("dy/dx:\n");
        text += QString::number((p2.y()-p1.y())/(p2.x()-p1.x()));
        text += QString("\nMin:\n") += QString::number(min);
        text += QString("\nMax:\n") += QString::number(max);
        text += QString("\nPeak to peak:\n") += QString::number(max-min);
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
    area.setXAxisLabel(xLabel);
    area.setYAxisLabel(yLabel);
}

void MainWindow::save(QString savingFile)
{
    QFile file(savingFile);
    file.open(QFile::WriteOnly);

    QJsonObject mainObject;

    mainObject["version"] = "1.0";

    mainObject["inputFile"] = inputFile;
    mainObject["xLabel"] = xLabel;
    mainObject["yLabel"] = yLabel;

    qreal xMin, xMax, yMin, yMax;
    area.getView(xMin, xMax, yMin, yMax);
    mainObject["xMin"] = xMin;
    mainObject["xMax"] = xMax;
    mainObject["yMin"] = yMin;
    mainObject["yMax"] = yMax;

    QJsonArray series;

    for (int i=0; i<dataSeries.size(); i++)
    {
        QJsonObject dataArray;
        dataArray["index"] = i;
        dataArray["name"] = dataSeries[i]->name;
        dataArray["color"] = dataSeries[i]->color.name();

        QJsonArray selectedPoints;
        for (int j=0; j<dataSeries[i]->selectedPoints.rowCount(); j++)
        {
            selectedPoints.append(dataSeries[i]->selectedPoints.item(j, 0)->data().toInt());
        }
        dataArray["selectedPoints"] = selectedPoints;
        series.append(dataArray);
    }
    mainObject["dataSeries"] = series;

    QJsonDocument doc(mainObject);

    file.write(doc.toJson());
}

bool MainWindow::restore(QString savedFile)
{
    QFile file(savedFile);
    file.open(QFile::ReadOnly);

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

    if (doc.isNull()) return false;

    QJsonObject mainObject = doc.object();

    inputFile = mainObject["inputFile"].toString();
    dataSeries.loadFromFile(inputFile);
    area.setDataSeries(&dataSeries);
    for (int i=0; i<dataSeries.size(); i++)
    {
        QPixmap pixmap(20, 20);
        pixmap.fill(dataSeries[i]->color);
        QIcon icon(pixmap);
        ui->seriesCombo->addItem(icon, dataSeries[i]->name, i);
    }

    xLabel = mainObject["xLabel"].toString();
    yLabel = mainObject["yLabel"].toString();
    area.setXAxisLabel(xLabel);
    area.setYAxisLabel(yLabel);

    qreal xMin, xMax, yMin, yMax;

    xMin = mainObject["xMin"].toDouble();
    xMax = mainObject["xMax"].toDouble();
    yMin = mainObject["yMin"].toDouble();
    yMax = mainObject["yMax"].toDouble();

    area.defineView(xMin, xMax, yMin, yMax);

    QJsonArray series = mainObject["dataSeries"].toArray();
    for (int i=0; i<series.size(); i++)
    {
        QJsonObject dataArray = series[i].toObject();
        int index = dataArray["index"].toInt();
        if (index < dataSeries.size())
        {
            dataSeries[index]->name = dataArray["name"].toString();
            QColor color(dataArray["color"].toString());
            dataSeries[index]->color = color;

            QPixmap pixmap(20, 20);
            pixmap.fill(color);
            QIcon icon(pixmap);
            ui->seriesCombo->setItemText(index, dataSeries[index]->name);
            ui->seriesCombo->setItemIcon(index, icon);

            QJsonArray selectedPoints = dataArray["selectedPoints"].toArray();
            for (int j=0; j<selectedPoints.size(); j++)
            {
                on_measurePointAdded(index, selectedPoints[j].toInt());
            }
            dataArray["selectedPoints"] = selectedPoints;
        }
    }

    area.update();
    return true;
}

void MainWindow::on_actionOpen_triggered()
{
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    QStringList filters;
    filters << "Text files (*.txt *.data *)"
            << "QSP files (*.qsp)";

    fileDialog.setNameFilters(filters);
    fileDialog.show();
}

void MainWindow::on_actionSave_triggered()
{
    if (savedFile.length() == 0)
    {
        exporting = false;
        fileDialog.setAcceptMode(QFileDialog::AcceptSave);
        fileDialog.setNameFilter("QSP files (*.qsp)");
        fileDialog.show();
    }
    else
        save(savedFile);
}

void MainWindow::on_fileDialogAccepted(QString fileName)
{
    if (fileDialog.acceptMode() == QFileDialog::AcceptOpen)
    {
        loadFile(fileName);
    }

    else if (fileDialog.acceptMode() == QFileDialog::AcceptSave)
    {
        if (exporting)
        {
            QSvgGenerator gen;
            gen.setFileName(fileName);
            gen.setSize(area.size());
            gen.setViewBox(QRect(0, 0, area.width(), area.height()));

            QPainter painter;
            painter.begin(&gen);
            painter.setFont(QFont(area.fontInfo().family(), area.fontInfo().pixelSize()));
            area.renderView(painter);

            exporting = false;
        }
        else
            save(fileName);
    }
}

void MainWindow::on_actionExport_triggered()
{
    exporting = true;
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setNameFilter("SVG files (*.svg)");
    fileDialog.show();
}
