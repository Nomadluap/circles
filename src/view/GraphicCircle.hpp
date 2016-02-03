#ifndef GRAPHICCIRCLE_HPP
#define GRAPHICCIRCLE_HPP

#include <QGraphicsItem>

#include "packing/Circle.hpp"
#include <memory>

using namespace Circles;

namespace Circles{
    namespace Ui{

    /**
     * Defines a visual circle object in a packingView. Each GraphicCircle corresponds to a single Circle, and therefore
     * node, in a packing. Once created, a circle's geometry is immutable. Therefore, after a repack or re-layout,
     * the circle will have to be destroyed and re-created to have its position updated.
     */
    class GraphicCircle: public QGraphicsItem
    {
    public:
        /**
         * Construct a default GraphicsCircle with center at (0, 0) and radius (1, 0).
         */
        GraphicCircle();

        /**
         * Construct a GraphicsCircle for a specified packing circle. Note that the Packing circle is
         * deep-copied and therefore the scene does not respond to changes in the packing's geometry.
         * @param c the assosciated PackingCircle.
         */
        GraphicCircle(const Packing::Circle& c);

        //inherited from QGraphicsItem
        QRectF boundingRect() const override;
        QPainterPath shape() const override;
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget=0) override;

        /**
         * Return the correpsonding graph node of this GraphicsCircle.
         * @return index of node on graph.
         */
        int graphIndex() const;


    private:

        /// Euclidean radius of the circle.
        qreal radius_;

        /// Node in the underlying graph that the circle represents.
        int index_;
    };

    }
}


#endif // GRAPHICCIRCLE_HPP
