#ifndef DATASERIES_H
#define DATASERIES_H

#include <QFile>
#include <QStringList>
#include <dataarray.h>

class DataSeries : public QList<DataArray*>
{
public:
    DataSeries();
    ~DataSeries();

    int loadFromFile(QString filename);

    void xMinMax(float& min, float& max);
    void yMinMax(float& min, float& max);

signals:

public slots:

private:

};

#endif // DATASERIES_H
