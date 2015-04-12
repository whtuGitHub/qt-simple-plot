#include "dataarray.h"
#include <cstdlib>

DataArray::DataArray()
{
    int r = rand() % 256;
    int g = rand() % 256;
    int b = rand() % 256;
    color = QColor(r, g, b);
}

DataArray::~DataArray()
{

}

void DataArray::xMinMax(float& min, float& max)
{
    min = max = at(0).x();

    for (int i=1; i<size(); i++)
    {
        if (at(i).x() > max) max = at(i).x();
        if (at(i).x() < min) min = at(i).x();
    }
}

void DataArray::yMinMax(float& min, float& max, int from, int to)
{
    to++;

    if (from < 0)
        from = 0;
    if (to < 0 || to > size())
        to = size();

    min = max = at(from).y();

    for (int i=from; i<to; i++)
    {
        if (at(i).y() > max) max = at(i).y();
        if (at(i).y() < min) min = at(i).y();
    }
}
