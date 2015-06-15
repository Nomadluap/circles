#include "Packing.hpp"
#include "Circle.hpp"
#include <QtWidgets>
#include "Hyperbolic.hpp"
#include <cmath>
Circle::Circle(Node *n, Packing *p)
{
    this->node = n;
    this->parent = p;
    if(this->parent->getType() == PackingType::EuclideanPacking){
        this->setPos(this->node->getPosition());
    }
    else{
        this->setPos(Hyperbolic::proj(this->node->getPosition()));
    }
}

QRectF Circle::boundingRect() const
{
    if(this->parent->getType() == PackingType::EuclideanPacking){
        return this->boundingRect_euclidean();
    }
    else{
        return this->boundingRect_hyperbolic();
    }
}

void Circle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(this->parent->getType() == PackingType::EuclideanPacking){
        this->paint_euclidean(painter, option, widget);
    }
    else{
        this->paint_hyperbolic(painter, option, widget);
    }
}

QRectF Circle::boundingRect_euclidean() const
{
    qreal radius = this->node->getRadius();
    return QRectF(-radius, -radius, 2*radius, 2*radius);
}

QRectF Circle::boundingRect_hyperbolic() const
{
    qreal radius = this->node->getRadius();
    //get left extent of the circle
    QPointF center = this->node->getPosition();
    qreal theta = atan2(center.y(), center.x());

    qreal c = sqrt(QPointF::dotProduct(center, center));
    qreal l = Hyperbolic::proj(QPointF(c - radius, 0)).x();
    qreal r = Hyperbolic::proj(QPointF(c + radius, 0)).x();
    qreal actualCenterMag = (l + r) / 2.0;
    QPointF actualCenter = QPointF(actualCenterMag * cos(theta),
                                 actualCenterMag * sin(theta));
    actualCenter -= center;

    qreal R = fabs((l - r) / 2.0);

    qreal x = actualCenter.x();
    qreal y = actualCenter.y();
    return QRectF(x-R, y-R, x+R, y+R);

}

void Circle::paint_euclidean(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    Q_UNUSED(option)
    //get the scale of the scene
    qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    qreal radius = this->node->getRadius();
    if(parent->getDrawCircles()){
        painter->setPen(QPen(Qt::black, BORDER_THICKNESS/lod ));
        painter->setBrush(QBrush(this->node->getColor()));
        painter->drawEllipse(QPointF(0, 0), radius, radius);
    }
    if(parent->getDrawCenters()){
        painter->setPen(QPen(Qt::black, 2/lod));
        painter->drawLine(QPointF(0, radius/4.0), QPointF(0, -radius/4.0));
        painter->drawLine(QPointF(radius/4.0, 0), QPointF(-radius/4.0, 0));
    }
    if(parent->getDrawIndicies()){
        QFont font("Times", 10);
        font.setStyleStrategy(QFont::ForceOutline);
        painter->setFont(font);
        painter->save();
        painter->scale(1.0/lod, 1.0/lod);
        painter->drawText(QPointF(4, -4), QString("%1").arg(this->node->getId()));
        painter->restore();
    }
}

void Circle::paint_hyperbolic(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    Q_UNUSED(option)
    qreal actualRadius = this->hyp_getActualRadius();
    QPointF actualCenter = this->hyp_getActualCenter();

    qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());

    if(parent->getDrawCircles()){
        painter->setPen(QPen(Qt::black, BORDER_THICKNESS/lod));
        painter->setBrush(QBrush(this->node->getColor()));
        painter->drawEllipse(actualCenter, actualRadius, actualRadius);
    }

    if(parent->getDrawCenters()){
        painter->setPen(QPen(Qt::black, 2/lod));
        painter->drawLine(actualCenter + QPointF(0, actualRadius/4.0),
                          actualCenter + QPointF(0, -actualRadius/4.0));

        painter->drawLine(actualCenter + QPointF(actualRadius/4.0, 0),
                          actualCenter + QPointF(-actualRadius/4.0, 0));
    }

    if(parent->getDrawIndicies()){
        QFont font("Times", 10);
        font.setStyleStrategy(QFont::ForceOutline);
        painter->setFont(font);
        painter->save();
        painter->scale(1.0/lod, 1.0/lod);
        painter->drawText(actualCenter, QString("%1").arg(this->node->getId()));
        painter->restore();
    }
}

QPointF Circle::hyp_getActualCenter()
{
    qreal radius = this->node->getRadius();
    //get left extent of the circle
    QPointF center = this->node->getPosition();
    qreal theta = atan2(center.y(), center.x());

    qreal c = sqrt(QPointF::dotProduct(center, center));
    qreal l = Hyperbolic::proj(QPointF(c - radius, 0)).x();
    qreal r = Hyperbolic::proj(QPointF(c + radius, 0)).x();
    qreal actualCenterMag = (l + r) / 2.0;
    QPointF actualCenter = QPointF(actualCenterMag * cos(theta),
                                 actualCenterMag * sin(theta));
    actualCenter -= center;
    return actualCenter;
}

qreal Circle::hyp_getActualRadius()
{
    qreal radius = this->node->getRadius();
    //get left extent of the circle
    QPointF center = this->node->getPosition();

    qreal c = sqrt(QPointF::dotProduct(center, center));
    qreal l = Hyperbolic::proj(QPointF(c - radius, 0)).x();
    qreal r = Hyperbolic::proj(QPointF(c + radius, 0)).x();
    qreal actualRadius = fabs((l - r) / 2.0);
    return actualRadius;

}



const qreal Circle::BORDER_THICKNESS = 2.0;
