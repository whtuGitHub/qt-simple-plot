#ifndef DATAARRAY_H
#define DATAARRAY_H

#include <QObject>
#include <QDebug>
#include <QColor>
#include <QList>
#include <QPointF>
#include <QStandardItemModel>

class DataArray : public QList<QPointF>
{
public:
    DataArray();
    ~DataArray();

    void xMinMax(float& min, float& max);
    void yMinMax(float& min, float& max, int from=-1, int to=-2);

    QColor color;
    QString name;
    QStandardItemModel selectedPoints;
};

#endif // DATAARRAY_H
