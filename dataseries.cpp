#include "dataseries.h"

DataSeries::DataSeries()
{

}

DataSeries::~DataSeries()
{

}

int DataSeries::loadFromFile(QString filename)
{
    while (size() > 0)
        removeFirst();

    QFile file(filename);
    if (!file.open(QFile::ReadOnly))
        return 1;

    QString stringLine = QString(file.readLine()).remove("\n");
    QStringList line = stringLine.split(" ", QString::SkipEmptyParts);

    if ((line.size() % 2) == 0)
    {
        for (int i=0; i<line.size(); i=i+2)
        {
            DataArray* newDataArray = new DataArray();
            this->append(newDataArray);
            newDataArray->append(QPointF(line[i].toFloat(), line[i+1].toFloat()));

            newDataArray->name = QString("Series ").append(QString().setNum(i/2));
        }

        while (!file.atEnd())
        {
            QStringList line = QString(file.readLine()).split(" ", QString::SkipEmptyParts);
            for (int i=0; i<size(); i++)
            {
                if (2*i+1 < line.size())
                    at(i)->append(QPointF(line[2*i].toFloat(), line[2*i+1].toFloat()));
            }
        }

        return 0;
    }
    return 1;
}

void DataSeries::xMinMax(float& min, float& max)
{
    at(0)->xMinMax(min, max);

    float tmpMin, tmpMax;

    for (int i=1; i<size(); i++)
    {
        at(i)->xMinMax(tmpMin, tmpMax);

        if (tmpMax > max) max = tmpMax;
        if (tmpMin < min) min = tmpMin;
    }
}

void DataSeries::yMinMax(float& min, float& max)
{
    at(0)->yMinMax(min, max);

    float tmpMin, tmpMax;

    for (int i=1; i<size(); i++)
    {
        at(i)->yMinMax(tmpMin, tmpMax);

        if (tmpMax > max) max = tmpMax;
        if (tmpMin < min) min = tmpMin;
    }
}
