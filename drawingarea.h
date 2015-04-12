#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include <QWidget>
#include <QPainter>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QSettings>
#include <dataseries.h>

class DrawingArea : public QWidget
{
    Q_OBJECT
public:
    explicit DrawingArea(QWidget *parent = 0);
    ~DrawingArea();

    void setDataSeries(DataSeries* series);
    void setActiveSeries(int index);
    void renderView(QPainter &painter);

protected:
    void wheelEvent(QWheelEvent* event);
    void mouseMoveEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    void calcAxis(float min, float max, float& intMin, float& intMax,
                  float& delta, float& subMin, float& subMax, float& subDelta);

    QPointF pointToLocal(QPointF point);
    QPointF localToPoint(QPointF local);

signals:
    void measurePointAdded(int series, int pointIndex);

private:
    DataSeries* dataSeries;
    int selectedPoint, selectedArray;
    float xMin, xMax, yMin, yMax;
    int reservedHeight, reservedWidth;
    QPointF lastPosition;
    QSettings settings;

signals:

public slots:
};

#endif // DRAWINGAREA_H
