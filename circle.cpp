#include "circle.h"

#include <QTWidgets>
#include <cmath>

Circle::Circle(qreal x, qreal y, qreal r):
    Circle::Circle(QPointF(x, y), r){}


Circle::Circle(QPointF center, qreal radius){
    this->center = center;
    this->setPos(this->center);
    this->radius = radius;

    fillColor = QColor(255, 255, 255);
}

Circle::~Circle(){
}

Circle *Circle::tangentTo(Circle *c, qreal angle, qreal radius){
    QPointF p0 = c->getCenter();
    qreal l = radius + c->getRadius();
    qreal x = l * cos(angle);
    qreal y = l * sin(angle);
    QPointF p = p0 + QPointF(x, y);
    Circle *circ = new Circle(p, radius);
    return circ;
}

Circle *Circle::tangentTo(Circle *cA, Circle *cB, qreal radius){
    QPointF pA = cA->getCenter();
    QPointF pB = cB->getCenter();
    qreal theta0 = std::atan2(pB.y() - pA.y(), pB.x() - pA.x());
    qreal rc = radius;
    qreal ra = cA->getRadius();
    qreal rb = cB->getRadius();
    //compute theta
    qreal arg = (ra + rb)*(ra + rb) + (ra + rc)*(ra + rc) - (rb + rc)*(rb + rc);
    arg /= (2.0 * (ra + rb) * (ra + rc));
    qreal theta = std::acos(arg);
    //final offsets
    qreal x = (ra + rc) * std::cos(theta + theta0);
    qreal y = (ra + rc) * std::sin(theta + theta0);
    //and the final center
    QPointF center = pA + QPointF(x, y);
    Circle *circ = new Circle(center, radius);
    return circ;
}

//Getters and setters
void Circle::setCenter(QPointF p){
    this->center = p;
    this->setPos(this->center);
}

QPointF Circle::getCenter(){
    return this->center;
}

void Circle::setRadius(qreal r){
    this->radius = r;
}

qreal Circle::getRadius(){
    return this->radius;
}

void Circle::setCentermarkVisible(bool visible){
    this->showCenter = visible;
}

bool Circle::getCentermarkVisible(){
    return this->showCenter;
}

int Circle::getIndex(){
    return this->index;
}

void Circle::setColor(QColor c){
    this->fillColor = c;
}

QColor Circle::getColor(){
    return this->fillColor;
}

void Circle::setThickness(qreal t){
    this->borderThickness = t;
}

qreal Circle::getThickness(){
    return this->borderThickness;
}

QRectF Circle::boundingRect() const {
    return QRectF(0, 0, this->radius * 2.0, this->radius * 2.0);
}

QPainterPath Circle::shape() const {
    QPainterPath path;
    path.addEllipse(0, 0, this->radius * 2.0, this->radius * 2.0);
    return path;
}

void Circle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(widget)
    Q_UNUSED(option)
    //draw the actual circle
    painter->setPen(QPen(Qt::black, this->borderThickness));
    painter->setBrush(QBrush(this->fillColor));
    painter->drawEllipse(QPointF(0, 0), radius, radius);
    //draw centermark if option is set
    if(showCenter){
        painter->setPen(QPen(Qt::black, 1));
        painter->drawLine(QPointF(0, radius/3.0), QPointF(0, -(radius/3.0)));
        painter->drawLine(QPointF(radius/3.0, 0), QPointF(-(radius/3.0), 0));
    }

}

