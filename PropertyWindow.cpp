#include "PropertyWindow.hpp"
#include "ui_PropertyWindow.h"
#include <qDebug>

PropertyWindow::PropertyWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PropertyWindow)
{
    ui->setupUi(this);
}

PropertyWindow::~PropertyWindow()
{
    delete ui;
}

void PropertyWindow::setNode(Node *n)
{
    qDebug() << "PropertyWindow: got new focus node: id" << n->getId();
    this->n = n;
    this->refresh();
}

void PropertyWindow::refresh()
{
    ui->txtId->setText(QString("%1").arg(n->getId()));
    ui->txtRadius->setText(QString("%1").arg(n->getRadius()));
    ui->txtX->setText(QString("%1").arg(n->getPosition().x()));
    ui->txtY->setText(QString("%1").arg(n->getPosition().y()));
    ui->txtFullFlower->setText(n->hasFullFlower() ? "True" : "False");

    //neibhours
    QList<Node*> neibhours = n->getNeibhours();
    ui->txtNeibhours->setPlainText("");
    for(Node* p: neibhours){
        ui->txtNeibhours->appendPlainText(QString("%1 \n").arg(p->getId()));
    }

}
