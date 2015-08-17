#include "ShapeSelector.hpp"
#include "Node.hpp"
#include "Packing.hpp"
#include <QList>
#include <QRectF>
#include "ui_ShapeSelector.h"
#include <QMessageBox>

ShapeSelector::ShapeSelector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShapeSelector)
{
    ui->setupUi(this);
    QList<Node*> nodes = Node::generateHexArray(QRectF(-0.5, -0.5, 2.0, 2.0), 0.05);
    this->packing = new Packing(nodes, PackingType::EuclideanPacking);
    this->packing->setDrawLinks(false);
    this->packing->setDrawIndicies(false);
    this->packing->setDrawCenters(false);
    this->packing->setDrawBoundary(false);


    ui->view->setRenderHint(QPainter::Antialiasing, true);
    ui->view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    ui->view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    qreal xscale = ui->view->size().width();
    qreal yscale = ui->view->size().height();
    ui->view->scale(xscale, yscale);
    ui->view->setSceneRect(0.0, 0.0, 1.0, 1.0);
    ui->view->setScene(this->packing);

    //connections
    connect(ui->btnRadiusSet, &QPushButton::clicked, this, &ShapeSelector::circleResize);
    connect(ui->btnCancel, &QPushButton::clicked, this, &ShapeSelector::close);
    connect(ui->btnAppend, &QPushButton::clicked, this, &ShapeSelector::manualAddVertex);

}

ShapeSelector::~ShapeSelector()
{
    delete ui;
    delete packing;
}

void ShapeSelector::manualAddVertex()
{
    QString s = ui->txtVertex->text();
    QStringList args = s.split(',');
    if(args.length() != 2){
        QMessageBox().warning(this, "Invalid Format",
                              "Vertex must be two comma-seperated numbers");
        return;
    }
    //parse doubles
    bool xOK = true;
    bool yOK = true;
    qreal x = args[0].toDouble(&xOK);
    qreal y = args[1].toDouble(&yOK);
    if(!xOK || !yOK){
        QMessageBox().warning(this, "Invalid Format", "Could not parse numbers");
        return;
    }
    this->addVertex(QPointF(x, y));
}

void ShapeSelector::deleteSelected()
{

}

void ShapeSelector::cullPacking()
{

}

void ShapeSelector::accept()
{

}

void ShapeSelector::circleResize()
{
   qreal r = ui->spinCircleRadius->value();
   //destroy nodes
   for(Node* n: this->packing->getNodes()) delete n;
   delete this->packing;
   QList<Node*> nodes = Node::generateHexArray(QRectF(-0.5, -0.5, 2.0, 2.0), r);
   this->packing = new Packing(nodes, PackingType::EuclideanPacking);
   this->packing->setDrawLinks(false);
   this->packing->setDrawIndicies(false);
   this->packing->setDrawCenters(false);
   this->packing->setDrawBoundary(false);
   ui->view->setScene(this->packing);

}

void ShapeSelector::addVertex(QPointF pos)
{
    this->vertices.append(pos);
    ui->lstVertices->
}

void ShapeSelector::resizeEvent(QResizeEvent *event)
{
    ui->view->resetMatrix();
    Q_UNUSED(event);
    qreal xscale = ui->view->size().width();
    qreal yscale = ui->view->size().height();
    ui->view->scale(xscale*0.95, yscale*0.95);
}


