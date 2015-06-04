#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QGraphicsScene>
#include "Packing.hpp"
#include "EuclideanPacking.hpp"


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
    void setZoom(int zoom);
private:
    Ui::MainWindow *ui;
    Packing *p;
};

#endif // MAINWINDOW_H
