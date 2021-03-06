#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QGraphicsScene>
#include "../graphics/Packing.hpp"
#include "ShapeSelector.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void openFile();
    void openShapeSelector();
    void shapeSelectorClosed();
    void processShapeSelectorPacking(Packing *p);
private:
    Ui::MainWindow *ui;
    Packing *p;
    ShapeSelector *shapeSelector = nullptr;

};

#endif // UI_MAINWINDOW_H
