#ifndef SHAPESELECTOR_HPP
#define SHAPESELECTOR_HPP

#include <QWidget>
#include <QResizeEvent>
#include <QList>
#include <QPointF>
#include "Packing.hpp"

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


private:
    void addVertex(QPointF pos);
    virtual void resizeEvent(QResizeEvent *event);

    Ui::ShapeSelector *ui;
    QList<QPointF> vertices;
    Packing *packing;
};

#endif // SHAPESELECTOR_HPP
