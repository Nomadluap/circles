#ifndef FIXEDARGRAPHICSVIEW_HPP
#define FIXEDARGRAPHICSVIEW_HPP

#include <QObject>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>

class FixedARGraphicsView : public QGraphicsView
{
public:
    FixedARGraphicsView(QWidget *parent = nullptr);
    FixedARGraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr);

    virtual int heightForWidth(int w) const {return w;}
    virtual bool hasHeightForWidth() const {return true;}

protected:
    virtual void resizeEvent(QResizeEvent *event);
};

#endif // FIXEDARGRAPHICSVIEW_HPP
