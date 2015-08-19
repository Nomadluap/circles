#ifndef PROPERTYWINDOW_HPP
#define PROPERTYWINDOW_HPP

#include <QWidget>
#include <QDialog>
#include "Node.hpp"

namespace Ui {
class PropertyWindow;
}

class PropertyWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PropertyWindow(QWidget *parent = 0);
    ~PropertyWindow();

public slots:
    /**
     * Sets the node which is displayed in the property window.
     * @param n Node to display.
     */
    void setNode(Node* n);
    /**
     * Updates the visual display using information from the currently selected
     * node.
     */
    void refresh();
private:
    Ui::PropertyWindow *ui;
    Node *n = nullptr;
};

#endif // UI_PROPERTYWINDOW_HPP
