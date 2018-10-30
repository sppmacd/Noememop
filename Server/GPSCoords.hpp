#pragma once

#include "config.hpp"

typedef Vector2<double> Vector2d;

namespace pms
{
    class GPSCoords
    {
        double nsDeg;
        double ewDeg;

    public:
        GPSCoords(double ns, double ew);
        double getNS();
        double getEW();
        GPSCoords& setNS(double ns);
        GPSCoords& setEW(double ew);
        
        GPSCoords& add(GPSCoords coords);
        double distance(GPSCoords coords);
        GPSCoords& subtract(GPSCoords coords);
        Vector2d getKMs();
    };
}
