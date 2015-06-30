#ifndef SHAPESELECTOR_HPP
#define SHAPESELECTOR_HPP

#include <QDialog>

namespace Ui {
class ShapeSelector;
}

class ShapeSelector : public QDialog
{
    Q_OBJECT

public:
    explicit ShapeSelector(QWidget *parent = 0);
    ~ShapeSelector();

private:
    Ui::ShapeSelector *ui;
};

#endif // SHAPESELECTOR_HPP
