#ifndef SHAPESELECTOR_HPP
#define SHAPESELECTOR_HPP

#include <QWidget>
#include <QDialog>
#include <QResizeEvent>
#include <QList>
#include <QPointF>
#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include "../graphics/Packing.hpp"
#include "../graphics/SelectionVertex.hpp"

namespace Ui {
class ShapeSelector;
}

class ShapeSelector : public QDialog
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

    QPointF getCenter();

    void setupPolygon();

    Ui::ShapeSelector *ui;
    QList<SelectionVertex *> vertices;
    Packing *packing;
    QGraphicsPolygonItem *polygon = nullptr;


};

#endif // UI_SHAPESELECTOR_HPP
