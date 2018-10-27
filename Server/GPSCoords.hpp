#pragma once

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
    };
}
