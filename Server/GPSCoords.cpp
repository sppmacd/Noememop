#include "GPSCoords.hpp"

namespace pms
{
    GPSCoords::GPSCoords(double ns, double ew)
    : nsDeg(ns), ewDeg(ew)
    {
    }
    double GPSCoords::getNS()
    {
        return nsDeg;
    }
    double GPSCoords::getEW()
    {
        return ewDeg;
    }
    GPSCoords& GPSCoords::setNS(double ns)
    {
        nsDeg = ns;
        return *this;
    }
    GPSCoords& GPSCoords::setEW(double ew)
    {
        ewDeg = ew;
        return *this;
    }
}
