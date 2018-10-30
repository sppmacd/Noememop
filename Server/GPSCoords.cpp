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
    
    GPSCoords& GPSCoords::add(GPSCoords coords)
    {
        return GPSCoords(this.getNS()+coords.getNS(), this.getEW()+coords.getEW());
    }
    
    double GPSCoords::distance(GPSCoords coords)
    {
        GPSCoords sub = coords.subtract(this).getKMs();
        GPSCoords subabs(abs(dist.getNS()), abs(dist.getEW()));
        Vector2d subkms = distABS.getKMs();
        return sqrt(subkms.x*subkms.x+subkms.y*subkms.y); //the distance
    }
    
    GPSCoords& GPSCoords::subtract(GPSCoords coords)
    {
        return GPSCoords(this.getNS()-coords.getNS(), this.getEW()-coords.getEW()); 
    }
    
    Vector2d GPSCoords::getKMs()
    {
        float kmsX = getEW() / 360.f * 40075.014f; 
        float alpha = getNS();
        float r1 = 6371.008f; //radius of the Earth
        float r = sqrt(pow(r1, 2) - pow(r1 * cos(alpha), 2)); //radius of the Earth on this geographical width
        float kmsY = ((r * 6.28f) / 360.f) * 40075.014f;
        return Vector2f(kmsX, kmsY);
    }
}
