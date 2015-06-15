#ifndef HYPERBOLIC_HPP
#define HYPERBOLIC_HPP

#include <QWidget>

namespace Hyperbolic{

/**
 * @brief proj finds the image of the point z projected onto the unit disc
 * @param z the point in hyperbolic space to project onto the disc
 * @return the image of the projected point.
 */
QPointF proj(QPointF z);

}

#endif // HYPERBOLIC_HPP
