#ifndef SHAPESELECTORGRAPHICSVIEW_HPP
#define SHAPESELECTORGRAPHICSVIEW_HPP

#include <QObject>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPointF>

class ShapeSelectorGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    ShapeSelectorGraphicsView(QWidget *parent = nullptr);
    ShapeSelectorGraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr);

    virtual int heightForWidth(int w) const {return w;}
    virtual bool hasHeightForWidth() const {return true;}

signals:
    void gotClick(QPointF scenepos);

protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
};

#endif // SHAPESELECTORGRAPHICSVIEW_HPP
