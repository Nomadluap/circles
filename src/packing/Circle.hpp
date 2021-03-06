#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include <QPointF>
#include <memory>

namespace Circles{
    namespace Packing{

    /**
     * Abstract base class for EuclidCircle and HyperCircle
     */
    class Circle
    {
    public:
        virtual std::unique_ptr<Circle> clone() const = 0;
        Circle();
        Circle(int index);
        Circle(const QPointF& center, qreal radius, int index);
        Circle(const Circle& other);
        Circle(Circle&& other);
        Circle& operator=(const Circle& other);
        Circle& operator=(Circle&& other);
        virtual ~Circle() {}
        /**
         * Get the index of the graph node associated with this Circle.
         * @return index of the graph node.
         */
        int index() const;

        /**
         * Set the index of this circle
         * @param index the index of the associated node in the graph.
         */
        void setIndex(int index);

        /**
         * The center of the circle in it's local space.
         * @return The point of the center of the circle in local coordinates.
         */
        virtual QPointF center() const = 0;

        /**
         * The radius of the circle in its local space.
         * @return radius of the circle in its local space.
         */
        virtual qreal radius() const = 0;

        /**
         * The center of the circle when projected into euclidean space (ie as it is on the monitor).
         * For circles that exist in Euclidean space, this will be equal to its center()
         * @return the projected center of the circle.
         */
        virtual QPointF projCenter() const = 0;

        /**
         * The radius of the circle when projected into euclidean space (ie as it is on the monitor).
         * For circles that exist in Euclidean space, this will be equal to its radius().
         * @return the projected radius of the circle.
         */
        virtual qreal projRadius() const = 0;

        /**
         * Set the radius of the circle to the specified value.
         * @param r The new radius of the circle. (in local space)
         */
        virtual void setRadius(qreal r) = 0;

        /**
         * Attempt to set the center of the circle.
         * @param c the center to set
         * @return True if the center was set correctly. False otherwise.
         */
        virtual bool setCenter(QPointF c) = 0;

        /**
         * Order comparison for sorting in lists, etc.
         * @param lhs
         * @param rhs
         * @return
         */
        friend bool operator<(const Circle& lhs, const Circle& rhs);

        friend bool operator==(const Circle& lhs, const Circle& rhs);

    protected:
        int _index; // graph node index that corresponds to this circle.
        qreal _radius;
        QPointF _center;
    };

    bool operator<(const Circle& lhs, const Circle& rhs);
    bool operator==(const Circle& lhs, const Circle& rhs);

    }
}

#endif // CIRCLE_HPP
