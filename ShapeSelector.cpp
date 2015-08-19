#include "ShapeSelector.hpp"
#include "Node.hpp"
#include "Packing.hpp"
#include <QList>
#include <QRectF>
#include "ui_ShapeSelector.h"
#include <QMessageBox>
#include <QGLWidget>
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <typeinfo>

ShapeSelector::ShapeSelector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShapeSelector)
{
    ui->setupUi(this);
    this->setModal(false);
    QList<Node*> nodes = Node::generateHexArray(QRectF(-0.1, -0.1, 1.2, 1.2), 0.05);
    this->packing = new Packing(nodes, PackingType::EuclideanPacking);
    this->packing->setDrawLinks(false);
    this->packing->setDrawIndicies(false);
    this->packing->setDrawCenters(false);
    this->packing->setDrawBoundary(false);
    ui->view->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::DirectRendering)));

    ui->view->setRenderHint(QPainter::Antialiasing, true);
    ui->view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    ui->view->setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
    ui->view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    qreal xscale = ui->view->size().width();
    qreal yscale = ui->view->size().height();
    ui->view->scale(xscale, yscale);
    ui->view->setSceneRect(0.0, 0.0, 1.0, 1.0);
    ui->view->setScene(this->packing);

    this->setupPolygon();

    //connections
    connect(ui->btnRadiusSet, &QPushButton::clicked, this, &ShapeSelector::circleResize);
    connect(ui->btnCancel, &QPushButton::clicked, this, &ShapeSelector::reject);
    connect(ui->btnAppend, &QPushButton::clicked, this, &ShapeSelector::manualAddVertex);
    connect(ui->view, &ShapeSelectorGraphicsView::gotClick, this, &ShapeSelector::addVertex);
    connect(ui->btnCull, &QPushButton::clicked, this, &ShapeSelector::cullPacking);
    connect(ui->btnAccept, &QPushButton::clicked, this, &ShapeSelector::accept);

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
    ui->txtVertex->clear();

}

void ShapeSelector::deleteSelected()
{

}

void ShapeSelector::cullPacking()
{
    qDebug() << "Culling the packing...";
    //first get a list of nodes
    QList<Node *> nodes = this->packing->getNodes();
    QList<Node *> removedNodes;
    //look at each node's position to deterimine if its center lies in the poly
    for(Node* n: nodes){
        qDebug() << "Looking at node" << n->getId() << "pos" << n->getPosition();
        if(!this->polygon->contains(n->getPosition())){
            //remove teh node from the packing
            this->packing->delNode_fast(n);
            removedNodes.append(n);
            qDebug() << "Removing:" << n->getId();
        }
        else{
            qDebug() << "keeping:" << n->getId();
        }
    }
    for(Node* n: removedNodes){
        nodes.removeAll(n);
    }
    removedNodes.clear();
    for(Node* n: nodes){
        if(n->getNeibhourCount() == 1){
            this->packing->delNode_fast(n);
            removedNodes.append(n);
        }
    }
    for(Node* n: removedNodes){
        nodes.removeAll(n);
    }
    this->packing->refreshCircles();
    //this->packing->resetIds();
    this->packing->update();
    qDebug() << "Culling complete";
    ui->btnAccept->setEnabled(true);


    //find the node at the center
    int center = -1;
    Circle *c = nullptr;
    for(auto i: this->packing->items(this->getCenter())){
        c = dynamic_cast<Circle *>(i);
        if (c != nullptr) break;
    }
    if(c != nullptr){
        center = c->getNode()->getId();
        qDebug() << "Center circle is " << center;
        this->packing->centerCircleID = center;
    }
}

void ShapeSelector::accept()
{
    Packing *p = new Packing(this->packing);
    emit packingAccepted(p);
}

void ShapeSelector::circleResize()
{
   qreal r = ui->spinCircleRadius->value();
   //destroy nodes
   for(Node* n: this->packing->getNodes()) delete n;
   delete this->packing;
   QList<Node*> nodes = Node::generateHexArray(QRectF(-0.1, -0.1, 1.2, 1.2), r);
   this->packing = new Packing(nodes, PackingType::EuclideanPacking);
   this->packing->setDrawLinks(false);
   this->packing->setDrawIndicies(false);
   this->packing->setDrawCenters(false);
   this->packing->setDrawBoundary(false);
   ui->view->setScene(this->packing);

   this->polygon = new QGraphicsPolygonItem();
   this->packing->addItem(this->polygon);
   this->polygon->setVisible(false);

   //POLYGON PROPERTIES
   QPen p;
   p.setWidthF(0.005);
   p.setColor(QColor(255, 0, 0, 200));
   this->polygon->setPen(p);
   QBrush b(Qt::SolidPattern);
   b.setColor(QColor(255, 0, 0, 68));
   this->polygon->setBrush(b);

}

void ShapeSelector::addVertex(QPointF pos)
{
    qDebug() << "Adding new vertex at" << pos;
    auto s = new SelectionVertex(pos);
    this->vertices.append(s);
    this->packing->addItem(s);
    this->packing->update();
    QString label = QString("%1,\t %2").arg(pos.x()).arg(pos.y());
    ui->lstVertices->addItem(label);
    ui->view->invalidateScene();
    //if we have at least three vertices, then we can now also allow culling
    if(this->vertices.length() >= 3){
        qDebug() << "showing the polygon";
        ui->btnCull->setEnabled(true);
        //construct a QPolygon using the vertices
        QPolygonF p;
        for(auto v: this->vertices) p.append(v->pos());
        p.append(this->vertices.first()->pos());
        this->polygon->setPolygon(p);
        this->polygon->setZValue(this->packing->items().first()->zValue() + 1);
        this->polygon->setVisible(true);
    }
}

void ShapeSelector::resizeEvent(QResizeEvent *event)
{
    ui->view->resetMatrix();
    Q_UNUSED(event);
    qreal xscale = ui->view->size().width();
    qreal yscale = ui->view->size().height();
    ui->view->scale(xscale*0.995, yscale*0.995);
    //ui->view->scale(xscale*0.0, yscale*0.0);
}

QPointF ShapeSelector::getCenter()
{
    QPointF sum;
    for(SelectionVertex *a: this->vertices) sum += a->pos();
    sum /= this->vertices.length();
    qDebug() << "Got center at " << sum;
    return sum;
}

void ShapeSelector::setupPolygon()
{
    this->polygon = new QGraphicsPolygonItem();
    this->packing->addItem(this->polygon);
    this->polygon->setVisible(false);

    //POLYGON PROPERTIES
    QPen p;
    p.setWidthF(0.005);
    p.setColor(QColor(255, 0, 0, 200));
    this->polygon->setPen(p);
    QBrush b(Qt::SolidPattern);
    b.setColor(QColor(255, 0, 0, 68));
    this->polygon->setBrush(b);
}


