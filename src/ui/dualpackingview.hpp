#ifndef DUALPACKINGVIEW_HPP
#define DUALPACKINGVIEW_HPP

#include <QWidget>

namespace Ui {
class DualPackingView;
}

class DualPackingView : public QWidget
{
    Q_OBJECT

public:
    explicit DualPackingView(QWidget *parent = 0);
    ~DualPackingView();

private:
    Ui::DualPackingView *ui;
};

#endif // DUALPACKINGVIEW_HPP
