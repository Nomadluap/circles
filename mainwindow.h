#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QGraphicsScene>
#include "Packing.hpp"


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
    void openFile();
    void setDrawCircles(bool b);
    void setDrawCenters(bool b);
    void setDrawIndicies(bool b);
    void setDrawLinks(bool b);
    void test();
    void doLayout();
    void doRepack();
    void setDragMode();
    void setSelectMode();
private:
    Ui::MainWindow *ui;
    Packing *p;

};

#endif // MAINWINDOW_H
