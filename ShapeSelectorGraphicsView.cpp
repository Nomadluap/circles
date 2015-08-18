#include "ShapeSelectorGraphicsView.hpp"
#include <QResizeEvent>

ShapeSelectorGraphicsView::ShapeSelectorGraphicsView(QWidget *parent):
    QGraphicsView(parent)
{
    this->sizePolicy().setWidthForHeight(true);

}

ShapeSelectorGraphicsView::ShapeSelectorGraphicsView(QGraphicsScene *scene, QWidget *parent):
    QGraphicsView(scene, parent)
{
    this->sizePolicy().setWidthForHeight(true);
}



void ShapeSelectorGraphicsView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    //    this->resize(this->size().height(), this->size().height());
}

void ShapeSelectorGraphicsView::mousePressEvent(QMouseEvent *event)
{
    QPointF scenepos = this->mapToScene(event->pos());
    emit gotClick(scenepos);
}

