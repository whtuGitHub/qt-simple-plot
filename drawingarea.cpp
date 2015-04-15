#include "drawingarea.h"
#include <cmath>

DrawingArea::DrawingArea(QWidget *parent) : QWidget(parent)
{
    dataSeries = 0;
    reservedHeight = 40;
    reservedWidth = 50;

    selectedPoint = -1;
    selectedArray = -1;
}

DrawingArea::~DrawingArea()
{

}

void DrawingArea::setDataSeries(DataSeries *series)
{
    dataSeries = series;

    if (series != 0)
    {
        dataSeries->xMinMax(xMin, xMax);
        dataSeries->yMinMax(yMin, yMax);

        update();
    }
}

void DrawingArea::setActiveSeries(int index)
{
    selectedArray = index;
}

void DrawingArea::setSelectedPoint(int index)
{
    selectedPoint = index;
}

void DrawingArea::defineView(float xMin, float xMax, float yMin, float yMax)
{
    if (xMax > xMin)
    {
        this->xMin = xMin;
        this->xMax = xMax;
    }
    if (yMax > yMin)
    {
        this->yMin = yMin;
        this->yMax = yMax;
    }
}

void DrawingArea::getView(float &xMin, float &xMax, float &yMin, float &yMax)
{
    xMin = this->xMin;
    xMax = this->xMax;
    yMin = this->yMin;
    yMax = this->yMax;
}

void DrawingArea::paintEvent(QPaintEvent *event)
{
    if (dataSeries != 0)
    {
        QPainter painter;

        painter.begin(this);

        renderView(painter);
    }
}

void DrawingArea::wheelEvent(QWheelEvent *event)
{
    float zoom;
    if (event->angleDelta().y() < 0)
        zoom = 1.1;
    else zoom = 1/1.1;

    if (settings.value("zoomX", true).toBool() == true)
    {
        float xf = localToPoint(event->posF()).x();
        xMin = xf-(xf-xMin)*zoom;
        xMax = xf-(xf-xMax)*zoom;
    }

    if (settings.value("zoomY", true).toBool() == true)
    {
        float yf = localToPoint(event->posF()).y();
        yMin = yf-(yf-yMin)*zoom;
        yMax = yf-(yf-yMax)*zoom;
    }

    update();
}

void DrawingArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (settings.value("pan", true).toBool() == true)
        {
            lastPosition = event->pos();
            setCursor(QCursor(Qt::ClosedHandCursor));
        }
        else if (settings.value("measure").toBool() == true)
        {
            emit measurePointAdded(selectedArray, selectedPoint);
        }
    }
}

void DrawingArea::mouseMoveEvent(QMouseEvent * event)
{
    if (settings.value("pan", true).toBool() == true &&
            event->buttons() == Qt::LeftButton)
    {
        QPointF dP = localToPoint(event->pos())-localToPoint(lastPosition);

        xMin -= dP.x();
        xMax -= dP.x();
        yMin -= dP.y();
        yMax -= dP.y();

        update();

        lastPosition = event->pos();
    }

    else if (settings.value("measure").toBool() == true)
    {
        int nearest = 0;
        float distance = abs(pointToLocal(dataSeries->at(selectedArray)->at(0)).x()-event->x());;

        for (int i=1; i<dataSeries->at(selectedArray)->size(); i++)
        {
            float d = abs(pointToLocal(dataSeries->at(selectedArray)->at(i)).x()-event->x());
            if (d < distance)
            {
                distance = d;
                nearest = i;
            }
        }

        if (selectedPoint != nearest)
        {
            selectedPoint = nearest;
            update();
        }
    }
}

void DrawingArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (settings.value("pan", true).toBool() == true &&
            event->button() == Qt::LeftButton)
        setCursor(QCursor(Qt::OpenHandCursor));
}

void DrawingArea::renderView(QPainter &painter)
{
    painter.fillRect(0, 0, width(), height(), QColor(255,255,255));

    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen;

    QPainterPath path;

    for (int i=0; i<dataSeries->size(); i++)
    {
        path = QPainterPath();

        pen.setColor(dataSeries->at(i)->color);
        pen.setWidth(3);

        DataArray* array = dataSeries->at(i);

        int n = 0;
        if (xMin <= array->at(0).x()) n = 0;
        else
        {
            while (n < array->size()-1) {
                if (array->at(n).x() < xMin && array->at(n+1).x() > xMin)
                    break;
                n++;
            }
        }

        int m = array->size()-1;
        if (xMax >= array->last().x()) m = array->size()-1;
        else
        {
            while (m > 0) {
                if (array->at(m-1).x() < xMax && array->at(m).x() > xMax)
                    break;
                m--;
            }
        }

        path.moveTo(pointToLocal(array->at(n)));

        for (int j=n+1; j<m+1; j++) {
            path.lineTo(pointToLocal(array->at(j)));
        }

        painter.strokePath(path, pen);

        pen.setWidth(8);
        pen.setCapStyle(Qt::RoundCap);
        painter.setPen(pen);
        for (int j=0; j<array->selectedPoints.rowCount(); j++)
        {
            painter.drawPoint(pointToLocal(array->at(array->selectedPoints.item(j, 0)->data().toInt())));
        }
    }

    //measure
    if (selectedPoint > -1)
    {
        pen.setColor(dataSeries->at(selectedArray)->color);
        pen.setWidth(8);
        pen.setCapStyle(Qt::SquareCap);
        painter.setPen(pen);
        painter.drawPoint(pointToLocal(dataSeries->at(selectedArray)->at(selectedPoint)));
    }

    //axes
    painter.setRenderHint(QPainter::Antialiasing, false);

    pen.setColor(QColor(0,0,0));
    pen.setWidth(1);
    painter.setPen(pen);

    painter.fillRect(0, 0, reservedWidth, height(), QColor(255,255,255));
    painter.fillRect(0, height()-reservedHeight, width(), height(), QColor(255,255,255));

    float min, max, delta, subMin, subMax, subDelta;
    calcAxis(xMin, xMax, min, max, delta, subMin, subMax, subDelta);

    for (float x=min; x<=max; x+=delta)
    {
        path = QPainterPath();
        QPointF p = pointToLocal(QPointF(x, 0));
        p.setY(height()-reservedHeight+10);
        path.moveTo(p);
        p.setY(height()-reservedHeight);
        path.lineTo(p);
        painter.strokePath(path, pen);
        p.setY(height()-20);
        painter.drawText(p, QString::number(x));
    }

    if (subDelta != delta)
    {
        for (float x=subMin; x<=subMax; x+=subDelta)
        {
            path = QPainterPath();
            QPointF p = pointToLocal(QPointF(x, 0));
            p.setY(height()-reservedHeight+5);
            path.moveTo(p);
            p.setY(height()-reservedHeight);
            path.lineTo(p);
            painter.strokePath(path, pen);
        }
    }

    calcAxis(yMin, yMax, min, max, delta, subMin, subMax, subDelta);

    for (float y=min; y<=max; y+=delta)
    {
        path = QPainterPath();
        QPointF p = pointToLocal(QPointF(0, y));
        p.setX(reservedWidth-10);
        path.moveTo(p);
        p.setX(reservedWidth);
        path.lineTo(p);
        painter.strokePath(path, pen);
        p.setX(0);
        painter.drawText(p, QString::number(y));
    }

    if (subDelta != delta)
    {
        for (float y=subMin; y<=subMax; y+=subDelta)
        {
            path = QPainterPath();
            QPointF p = pointToLocal(QPointF(0, y));
            p.setX(reservedWidth-10);
            path.moveTo(p);
            p.setX(reservedWidth);
            path.lineTo(p);
            painter.strokePath(path, pen);
        }
    }

    path = QPainterPath();
    path.moveTo(reservedWidth, height()-reservedHeight);
    path.lineTo(width()-1, height()-reservedHeight);
    painter.strokePath(path, pen);

    path = QPainterPath();
    path.moveTo(reservedWidth, 1);
    path.lineTo(reservedWidth, height()-reservedHeight);
    painter.strokePath(path, pen);
}

void DrawingArea::calcAxis(float min, float max, float &intMin,
    float &intMax, float &delta, float &subMin, float &subMax, float &subDelta)
{
    intMin = subMin = floor(min);
    intMax = subMax = ceil(max);
    delta = subDelta = 1.0;

    int n = (intMax-intMin)/delta;

    if (n < 6)
    {
        while (n < 6)
        {
            subDelta = delta/10;

            for (float x=intMin; x <= intMax; x += subDelta)
            {
                if (x+subDelta > min && x < min)
                    subMin = x;
                if (x > max && x-subDelta < max)
                    subMax = x;
            }

            subMin = round(subMin/subDelta)*subDelta;
            subMax = round(subMax/subDelta)*subDelta;

            n = (subMax-subMin)/subDelta;

            if (n < 10)
            {
                delta = subDelta;
                intMin = subMin;
                intMax = subMax;
            }
        }
    }

    else if (n > 12)
    {
        subMin = intMin;
        subMax = intMax;
        subDelta = delta;

        for (int i=1; n > 12; i++)
        {
            float f = pow(10.0, i);
            intMin = floor(min/f)*f;
            intMax = ceil(max/f)*f;
            delta = delta*10;

            n = (intMax-intMin)/delta;

            if (n > 12)
            {
                subMin = intMin;
                subMax = intMax;
                subDelta = delta;
            }
        }
    }
    intMin += delta;
    intMax -= delta;
    subMin += subDelta;
    subMax -= subDelta;

    if ((intMax-intMin)/delta < 6)
        delta = delta/2;

    while (intMin-delta > min) intMin -= delta;
    while (intMax+delta < max) intMax += delta;
}

QPointF DrawingArea::pointToLocal(QPointF point)
{
    return QPointF(reservedWidth+(point.x()-xMin)/(xMax-xMin)*(width()-reservedWidth),
            (yMax-point.y())/(yMax-yMin)*(height()-reservedHeight));
}

QPointF DrawingArea::localToPoint(QPointF local)
{
    return QPointF(1.*(local.x()-reservedWidth)/(width()-reservedWidth)*(xMax-xMin)+xMin,
    yMax - 1.*local.y()/(height()-reservedHeight)*(yMax-yMin));
}
