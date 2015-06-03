#ifndef CIRCLE_H
#define CIRCLE_H

#include <QColor>
#include <QGraphicsItem>


class Circle : public QGraphicsItem
{
public:
    Circle(qreal x, qreal y, qreal r);
    Circle(QPointF center, qreal radius);
    ~Circle();
    //generator functions
    /**
     * @brief tangentTo Create a circle tangent to given circle
     * @param c Circle to be tangent to
     * @param angle Angle from horizontal to make circle center, in radians
     * @param radius radius of new circle
     * @return pointer to new Circle created
     */
    static Circle* tangentTo(Circle *c, qreal angle, qreal radius);
    /**
     * @brief tangentTo Create Circle tangent to two circles
     * @param cA First Circle
     * @param cB Second Circle
     * @param radius radius of new circle
     * @return pointer to new Circle created.
     */
    static Circle* tangentTo(Circle *cA, Circle *cB, qreal radius);

    void setCenter(QPointF p);
    QPointF getCenter();

    void setRadius(qreal r);
    qreal getRadius();

    void setCentermarkVisible(bool);
    bool getCentermarkVisible();

    void setIndexVisible(bool);
    bool getIndexVisible();

    void setIndex(int i);
    int getIndex();

    void setColor(QColor c);
    QColor getColor();

    void setThickness(qreal t);
    qreal getThickness();

    /**
     * @brief isTangent Determine if circles are tangent
     * @param c circle to test against
     * @return true if circles are tangent
     */
    bool isTangent(Circle *c);

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget ) Q_DECL_OVERRIDE;

protected:


private:
    const qreal EPSILON = 1e-06;
    QPointF center;
    qreal radius;
    bool showCenter = false;
    bool showIndex = true;
    int index = 1234; //in future will dereference assosciated node object.
    QColor fillColor;
    qreal borderThickness = 2.0;

};

#endif // CIRCLE_H
