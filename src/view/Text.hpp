#ifndef TEXT_HPP
#define TEXT_HPP
#include <QGraphicsItem>
#include "packing/Circle.hpp"
#include <QFont>
#include <QRectF>

namespace Circles{
    namespace View{
    class Text: public QGraphicsItem
    {
    public:
        Text(QMap<int, std::shared_ptr<Packing::Circle> > circles, QRectF boundingrect,  QGraphicsItem* parent = 0);
        ~Text();

        virtual QRectF boundingRect() const override;
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    private:
        QMap<int, std::shared_ptr<Packing::Circle> > circles;
        const QRectF boundingRect_;

    };
    }
}


#endif // TEXT_HPP
