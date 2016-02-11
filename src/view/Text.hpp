#ifndef TEXT_HPP
#define TEXT_HPP
#include <QGraphicsSimpleTextItem>

namespace Circles{
    namespace View{
    class Text: public QGraphicsSimpleTextItem
    {
    public:
        Text(QGraphicsItem* parent = 0);
        Text(const QString& text, QGraphicsItem* parent = 0);
        ~Text();

        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    };
    }
}


#endif // TEXT_HPP
