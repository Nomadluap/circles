#ifndef EUCLIDCIRCLE_HPP
#define EUCLIDCIRCLE_HPP

#include<packing/Circle.hpp>

namespace Circles{
    namespace Packing{

    /**
     * A circle on the euclidean plane.
     */
    class EuclidCircle : public Circle
    {
    public:
        /**
         * Construct an empty euclidean circle, centered at (0, 0) with radius 1.0, and index -1.
         */
        EuclidCircle();

        /**
         * Construct a euclidean circle with a given center , radius, and index.
         * @param center Center point of the circle, in euclidean space.
         * @param radius Radius of the circle.
         * @param index Index of corresponding node in the underlying graph.
         */
        EuclidCircle(const QPointF& center, qreal radius, int index);

        EuclidCircle(const EuclidCircle& other);

        EuclidCircle& operator=(const EuclidCircle& other);

        friend bool operator==(const EuclidCircle& lhs, const EuclidCircle& rhs);

        virtual QPointF center() const override;
        virtual qreal radius() const override;
        virtual QPointF projCenter() const override;
        virtual qreal projRadius() const override;

        virtual void setRadius(qreal r) override;
        virtual bool setCenter(QPointF c) override;

    };

    bool operator==(const EuclidCircle& lhs, const EuclidCircle& rhs);

    }
}

#endif // EUCLIDCIRCLE_HPP
