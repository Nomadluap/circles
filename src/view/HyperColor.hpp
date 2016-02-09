#ifndef HYPERCOLOR_HPP
#define HYPERCOLOR_HPP

#include <QColor>
#include <QPointF>

namespace Circles{
    namespace View{
    /**
     * Get the color of the hyperbolic disk at a specific color according to Logan Hoehn's ComplexColorTranslate code
     * @param p the point to query
     * @return The color at that point
     */
    QColor getHyperColor(QPointF p);
    }
}


#endif // HYPERCOLOR_HPP
