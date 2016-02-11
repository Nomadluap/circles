#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include "gui/ShapeSelector.hpp"

namespace Ui {
class MainWindow;
}
namespace Circles{
namespace GUI{
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openSelector();

private:

    std::shared_ptr<Circles::GUI::ShapeSelector> ss;
    Ui::MainWindow *ui;
};

}
}

#endif // MAINWINDOW_HPP
