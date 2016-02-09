#ifndef HYPERCIRCLE_HPP
#define HYPERCIRCLE_HPP

#include"packing/Circle.hpp"


using namespace Circles::Packing;

namespace Circles{
    namespace Packing{

    /**
     * A circle in the hyperbolic poincare disc.
     */
    class HyperCircle : public Circle
    {
    public:
        /**
         * Return a pointer to an exact copy of this circle.
         * @return
         */
        virtual std::unique_ptr<Circle> clone() const override;

        /**
         * Construct an empty euclidean circle, centered at (0, 0) with radius 1.0, and index -1.
         */
        HyperCircle();

        /**
         * Construct a Hyper Circle with specified index. Centered at the origin with radius 1.
         * @param index Index in the underlying graph.
         */
        HyperCircle(int index);

        /**
         * Construct a Hyperbolic circle with a given center , radius, and index.
         * @param center Center point of the circle, in hyperbolic disc space.
         * @param radius Hyperbolic Radius of the circle.
         * @param index Index of corresponding node in the underlying graph.
         */
        HyperCircle(const QPointF& center, qreal radius, int index);

        HyperCircle(const HyperCircle& other);

        HyperCircle& operator=(const HyperCircle& other);

        friend bool operator==(const HyperCircle& lhs, const HyperCircle& rhs);

        virtual QPointF center() const override;
        virtual qreal radius() const override;
        virtual QPointF projCenter() const override;
        virtual qreal projRadius() const override;

        virtual void setRadius(qreal r) override;
        virtual bool setCenter(QPointF c) override;

    };

    bool operator==(const HyperCircle& lhs, const HyperCircle& rhs);

    }
}
#endif // HYPERCIRCLE_HPP
