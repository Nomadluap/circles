#include "Hyperbolic.hpp"
#include <cmath>

QPointF Hyperbolic::proj(QPointF z)
{
    //first break down z into argument and magnitude
    qreal mag = sqrt(pow(z.x(), 2) + pow(z.y(), 2));
    qreal theta = atan2(z.y(), z.x());

    //special case for zero
    if(mag == 0.0) return QPointF(0, 0);

    //convert the magnitude
    qreal pmagg = 1.0 / ((2.0 / (cosh(mag) - 1.0)) + 1);
    qreal pmag = sqrt(pmagg);

    //now combine back into a point
    qreal x = pmag * cos(theta);
    qreal y = pmag * sin(theta);
    return QPointF(x, y);
}

