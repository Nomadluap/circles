#ifndef SHAPESELECTOR_HPP
#define SHAPESELECTOR_HPP

#include <QWidget>
#include <QResizeEvent>
#include <QList>
#include <QPointF>
#include "Packing.hpp"
#include "SelectionVertex.hpp"
#include <QGraphicsPolygonItem>
#include <QPolygonF>

namespace Ui {
class ShapeSelector;
}

class ShapeSelector : public QWidget
{
    Q_OBJECT
public:
    explicit ShapeSelector(QWidget *parent = 0);
    ~ShapeSelector();

signals:
    void packingAccepted(Packing *p);

private slots:
    void manualAddVertex();
    void deleteSelected();
    void cullPacking();
    void accept();
    void circleResize();
    void addVertex(QPointF pos);

private:
    virtual void resizeEvent(QResizeEvent *event);

    void setupPolygon();

    Ui::ShapeSelector *ui;
    QList<SelectionVertex *> vertices;
    Packing *packing;
    QGraphicsPolygonItem *polygon = nullptr;


};

#endif // SHAPESELECTOR_HPP
