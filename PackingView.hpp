#ifndef PACKINGVIEW_HPP
#define PACKINGVIEW_HPP

#include <QWidget>
#include <QWidget>
#include <QGraphicsScene>
#include "Packing.hpp"
#include "PropertyWindow.hpp"
#include "PackingType.hpp"

namespace Ui {
class PackingView;
}

class PackingView : public QWidget
{
    Q_OBJECT

public:
    explicit PackingView(QWidget *parent = 0);
    PackingView(Packing *p, QWidget *parent = 0);
    ~PackingView();

public slots:
    void setPacking(Packing *p);

private slots:
    void setZoom(int zoom);
    void setDrawCircles(bool b);
    void setDrawCenters(bool b);
    void setDrawIndicies(bool b);
    void setDrawLinks(bool b);
    void setDrawBoundary(bool b);
    void test();
    void doLayout();
    void doRepack();
    void setDragMode();
    void setSelectMode();
    void setPackingType(bool b); //TRUE is euclidean, false hyperbolic

private:
    Ui::PackingView *ui;
    Packing *packing;
    PropertyWindow *propWindow;
};

#endif // PACKINGVIEW_HPP
