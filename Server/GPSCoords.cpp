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

    GPSCoords GPSCoords::add(GPSCoords& coords)
    {
        return GPSCoords(this->getNS()+coords.getNS(), this->getEW()+coords.getEW());
    }

    double GPSCoords::distance(GPSCoords& coords)
    {
        return sqrt(pow(coords.getNS() - this->getNS(),2) + pow(cos(this->getNS()*3.141592/180)*(coords.getEW() - this->getEW()),2))*(40075.704/360);
    }

    GPSCoords GPSCoords::subtract(GPSCoords& coords)
    {
        return GPSCoords(this->getNS()-coords.getNS(), this->getEW()-coords.getEW());
    }

    Vector2d GPSCoords::getKMs()
    {
        /*double kmsX = getEW() / 360 * 40075.014;
        double alpha = getNS();
        double r1 = 6371.008; //radius of the Earth
        double r = sqrt(pow(r1, 2) - pow(r1 * cos(alpha/(180/3.141592)), 2)); //radius of the Earth on this geographical width
        double kmsY = ((r * 3.141592 * 2) / 360) * 40075.014;*/
        return Vector2d(0.0, 0.0); //deprecated
    }
}
