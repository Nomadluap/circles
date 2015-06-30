#include "Packing.hpp"
#include "Circle.hpp"
#include <QtWidgets>
#include <cmath>
Circle::Circle(Node *n, Packing *p)
{
    this->setFlag(ItemIsSelectable);
    this->node = n;
    this->parent = p;
    if(this->parent->getType() == PackingType::EuclideanPacking){
        this->setPos(this->node->getPosition());
    }
    else{
        this->setPos(hyp_getEuclideanCenter());
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

void Circle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "circle " << this->node->getId() << "got mouse press event";
    if(this->selectionState == SelectionState::Selected){
        this->selectionState = SelectionState::None;
        qDebug() << "State is now Unselected";
    }
    else{
        this->selectionState = SelectionState::Selected;
        qDebug() << "State is now selected";
    }
    this->update();
}

QRectF Circle::boundingRect_euclidean() const
{
    qreal radius = this->node->getRadius();
    return QRectF(-radius, -radius, 2*radius, 2*radius);
}

QRectF Circle::boundingRect_hyperbolic() const
{
    QPointF center = this->node->getPosition();

    qreal c = sqrt(QPointF::dotProduct(center, center));
    qreal r = this->node->getRadius();

    qreal s = (exp(r) - 1) / (exp(r) + 1);

    // now the isometry phi maps the real point c to the origin.
    //where phi(z) = (z-c)/(cz-1)
    qreal a = (s-c)/(s*c - 1);
    qreal b = (-s-c)/(-s*c - 1);

    //now get radius, just like in hyp_getEuclideanRadius()
    qreal R = fabs((a-b)/2.0);
    //now form the rect
    return QRectF(-R, -R, 2*R, 2*R);
//    return QRectF(-2*R, -2*R, 4*R, 24*R);

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
        painter->setBrush(QBrush(this->getSelectionColor()));
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
    qreal R = this->hyp_getEuclideanRadius();
    QPointF eCent = this->hyp_getEuclideanCenter();
    QPointF cent = this->node->getPosition();
    qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());

    if(parent->getDrawCircles()){
        painter->setPen(QPen(Qt::black, BORDER_THICKNESS/lod));
        painter->setBrush(QBrush(this->getSelectionColor()));
        painter->drawEllipse(QPointF(0, 0), R, R);
    }

    if(parent->getDrawCenters()){
        painter->setPen(QPen(Qt::black, 2/lod));
        painter->drawLine(cent - eCent + QPointF(0, R/4.0),
                          cent - eCent + QPointF(0, -R/4.0));

        painter->drawLine(cent - eCent + QPointF(R/4.0, 0),
                          cent - eCent + QPointF(-R/4.0, 0));
    }

    if(parent->getDrawIndicies()){
        QFont font("Times", 10);
        font.setStyleStrategy(QFont::ForceOutline);
        painter->setFont(font);
        painter->save();
        painter->scale(1.0/lod, 1.0/lod);
        painter->drawText(cent - eCent + QPointF(4, -4), QString("%1").arg(this->node->getId()));
        painter->restore();
    }
}

QPointF Circle::hyp_getEuclideanCenter()
{
    QPointF center = this->node->getPosition();
    qreal theta = atan2(center.y(), center.x());

    qreal c = sqrt(QPointF::dotProduct(center, center));
    qreal r = this->node->getRadius();

    qreal s = (exp(r) - 1) / (exp(r) + 1);

    // now the isometry phi maps the real point c to the origin.
    //where phi(z) = (z-c)/(cz-1)
    qreal a = (s-c)/(s*c - 1);
    qreal b = (-s-c)/(-s*c - 1);

    //now magnitude of euclidean center is at midpoint of a and b
    qreal m = (a+b)/2.0;
    return QPointF(m*cos(theta), m*sin(theta));
}

qreal Circle::hyp_getEuclideanRadius()
{
    QPointF center = this->node->getPosition();

    qreal c = sqrt(QPointF::dotProduct(center, center));
    qreal r = this->node->getRadius();

    qreal s = (exp(r) - 1) / (exp(r) + 1);

    // now the isometry phi maps the real point c to the origin.
    //where phi(z) = (z-c)/(cz-1)
    qreal a = (s-c)/(s*c - 1);
    qreal b = (-s-c)/(-s*c - 1);

    //now half of difference between a and b is the radius
    return fabs((a-b)/2.0);

}

QColor Circle::getSelectionColor()
{
    switch(this->selectionState){
    case SelectionState::None:
        return this->node->getColor();
    case SelectionState::Selected:
        return QColor(227,116,116);
    case SelectionState::Surrounded:
        return QColor(116,227,172);
    default:
        return this->node->getColor();
    }
}

Circle::SelectionState Circle::getSelectionState()
{
    return this->selectionState;
}

void Circle::setSelectionState(Circle::SelectionState s)
{
    this->selectionState = s;
    this->update();
}



const qreal Circle::BORDER_THICKNESS = 2.0;
