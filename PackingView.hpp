#ifndef PACKINGVIEW_HPP
#define PACKINGVIEW_HPP

#include <QWidget>
#include <QWidget>
#include <QGraphicsScene>
#include "Packing.hpp"

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
    void test();
    void doLayout();
    void doRepack();
    void setDragMode();
    void setSelectMode();

private:
    Ui::PackingView *ui;
    Packing *packing;
};

#endif // PACKINGVIEW_HPP
