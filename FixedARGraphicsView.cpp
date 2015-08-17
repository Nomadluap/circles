#include "FixedARGraphicsView.hpp"
#include <QResizeEvent>

FixedARGraphicsView::FixedARGraphicsView(QWidget *parent):
    QGraphicsView(parent)
{
    this->sizePolicy().setWidthForHeight(true);

}

FixedARGraphicsView::FixedARGraphicsView(QGraphicsScene *scene, QWidget *parent):
    QGraphicsView(scene, parent)
{
    this->sizePolicy().setWidthForHeight(true);
}



void FixedARGraphicsView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
//    this->resize(this->size().height(), this->size().height());
}

