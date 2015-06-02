#ifndef CIRCLE_H
#define CIRCLE_H

#include <QColor>
#include <QGraphicsItem>


class Circle : public QGraphicsItem
{
public:
    Circle(double x, double y, double r);
    ~Circle();

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget ) Q_DECL_OVERRIDE;

protected:


private:
    double x;
    double y;
    double r;
    QColor color;
};

#endif // CIRCLE_H
