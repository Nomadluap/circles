#ifndef SHAPESELECTOR_HPP
#define SHAPESELECTOR_HPP

#include <QWidget>

namespace Ui {
class ShapeSelector;
}

class ShapeSelector : public QWidget
{
    Q_OBJECT

public:
    explicit ShapeSelector(QWidget *parent = 0);
    ~ShapeSelector();

private:
    Ui::ShapeSelector *ui;
};

#endif // SHAPESELECTOR_HPP
