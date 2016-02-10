#include "HyperColor.hpp"

#include <memory>
#include <cmath>
#include <QDebug>

using namespace Circles;
using namespace Circles::View;

QColor Circles::View::getHyperColor(QPointF p)
{
    const qreal blending = 2.0;
    const qreal contrast1 = 0.5;
    const qreal contrast2 = 0.8;
    const qreal radius = 1.0;

    qreal x = p.x();
    qreal y = -p.y();
    qreal d = sqrt(QPointF::dotProduct(p, p)) / radius;
    qreal theta = atan2(y, x);

    qreal r = 0;
    qreal b = 0;
    qreal g = 0;

    if(x > 0){
        b = b + pow(cos(theta), blending);
    } else{
        r = r + pow(-cos(theta), blending);
        g = g + pow(-cos(theta), blending);
    }

    if(y > 0){
        r = r + pow(sin(theta), blending);
    } else{
        g = g + pow(-sin(theta), blending);
    }

    r = fmin(1.0, pow(r, contrast1) * pow(d, contrast2));
    g = fmin(1.0, pow(g, contrast1) * pow(d, contrast2));
    b = fmin(1.0, pow(b, contrast1) * pow(d, contrast2));

    if(d > 1.0){
        r = fmin(1, r + (1-r) * (d-1));
        g = fmin(1, g + (1-g) * (d-1));
        b = fmin(1, b + (1-b) * (d-1));
    }
    int rr = int(round(r*255));
    int gg = int(round(g*255));
    int bb = int(round(b*255));
    QColor c(rr, gg, bb);
    return std::move(c);
}
