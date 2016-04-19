#include "ShapeSelector.hpp"
#include "ui_ShapeSelector.h"
#include <typeinfo>
#include <QList>
#include <QRectF>
#include <QMessageBox>
#include <QGLWidget>
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QFileDialog>

#include "loader/CPShape.hpp"


using namespace Circles;
using namespace Circles::GUI;

ShapeSelector::ShapeSelector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShapeSelector)
{
    ui->setupUi(this);
    this->setModal(false);
//    // set up the packing for the first time
//    this->packing = Packing::EuclidPacking::generateHexArray(QRectF(-1.0, -1.0, 2.0, 2.0), 0.1);
//    this->packingView = std::make_shared<View::PackingView>(this->packing);
//    this->packingView->update();
//    this->packingView->setDrawCenters(false);
//    this->packingView->setDrawCircles(true);
//    this->packingView->setDrawColor(false);
//    this->packingView->setDrawConnectors(false);
//    this->packingView->setDrawIndices(false);


//    ui->view->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::DirectRendering)));

//    ui->view->setRenderHint(QPainter::Antialiasing, true);
//    ui->view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
//    ui->view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

//    qreal xscale = ui->view->size().width();
//    qreal yscale = ui->view->size().height();
//    //ui->view->scale(1.0/xscale, 1.0/yscale);
//    //ui->view->setSceneRect(-1.0, -1.0, 2.0, 2.0);
//    ui->view->setScene(this->packingView.get());
//    ui->view->ensureVisible(this->packingView->sceneRect());
//    ui->view->setTransform(QTransform());
    this->circleResize(0.1);

    this->setupPolygon();

    //connections
    connect(ui->btnRadiusSet, SIGNAL(clicked(bool)), this, SLOT(circleResize()));
    connect(ui->btnCancel, &QPushButton::clicked, this, &ShapeSelector::reject);
    connect(ui->btnAppend, &QPushButton::clicked, this, &ShapeSelector::manualAddVertex);
    connect(this->packingView.get(), &View::PackingView::gotMousePressEvent, this, &ShapeSelector::sceneMousePressEvent);
    connect(ui->btnCull, &QPushButton::clicked, this, &ShapeSelector::cullPacking);
    connect(ui->btnAccept, &QPushButton::clicked, this, &ShapeSelector::acceptPacking);
    connect(ui->btnDelete, &QPushButton::clicked, this, &ShapeSelector::deleteSelected);
    connect(ui->lstVertices, &QListWidget::currentRowChanged, this, &ShapeSelector::selectionChanged);
    connect(ui->btnOpenShapeFile, SIGNAL(clicked(bool)), this, SLOT(readShapeFile()));
    connect(ui->btnSaveShapeFile, SIGNAL(clicked(bool)), this, SLOT(writeShapeFile()));

}

ShapeSelector::~ShapeSelector()
{
    delete ui;
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
    //make sure a row is actually selected
    if(this->selectedRow < 0) return; //-1 means no selection

    auto v = this->vertices.at(selectedRow);
    v->setVisible(false);
    this->packingView->removeItem(v.get()); //destroys v
    this->vertices.removeOne(v);
    this->redefinePolygon();
    //and remove the item from the list
    ui->lstVertices->takeItem(this->selectedRow);
    this->selectedRow--;



}

void ShapeSelector::selectionChanged(int row)
{
    this->selectedRow = row;

}

void ShapeSelector::cullPacking()
{
    qDebug() << "Culling the packing...";
    // look at every circle and determine if its center lies in the polygon.
    for(auto c: this->packing->circles().values()){
        if(!this->polygon->contains(c->center())){
            this->packing->removeCircle(c->index());
        }
    }
    //now we need to go through the packing and find circles that have only one neighbour, and delete them too.
    // we need to keep on doing this until each circle has at least two neighbours.
    bool everyCircleHas2 = false;
    while(!everyCircleHas2){
        everyCircleHas2 = true;
        Graph::Graph graph(this->packing->graph());

        for(Node n: graph.getNodes()){
            if(graph.neighbours(n).length() < 2){
                everyCircleHas2 = false;
                this->packing->removeCircle(n);
            } // if
        } // for
    } // while

    qDebug() << "Culling complete";
    ui->btnAccept->setEnabled(true);
    // and update the view to reflect the new packing
    this->packingView->setPacking(this->packing);

}

void ShapeSelector::acceptPacking()
{

    emit packingAccepted(this->packing);
}

void ShapeSelector::circleResize()
{
   qreal r = ui->spinCircleRadius->value();
   this->circleResize(r);
}

void ShapeSelector::circleResize(qreal radius)
{
    this->resetPolygon();
   qreal r = radius;
   this->circleRadius = radius;
   //destroy nodes
   if(this->packingView != nullptr) disconnect(this->packingView.get(), &View::PackingView::gotMousePressEvent, this, &ShapeSelector::sceneMousePressEvent);
   this->packing = Packing::EuclidPacking::generateHexArray(QRectF(-1.0, -1.0, 2.0, 2.0), r);
   this->packingView = std::make_shared<View::PackingView>(this->packing);
   this->packingView->setDrawCenters(false);
   this->packingView->setDrawCircles(true);
   this->packingView->setDrawColor(false);
   this->packingView->setDrawConnectors(false);
   this->packingView->setDrawIndices(false);
   connect(this->packingView.get(), &View::PackingView::gotMousePressEvent, this, &ShapeSelector::sceneMousePressEvent);

   ui->view->setScene(this->packingView.get());
   ui->view->centerOn(0, 0);

   this->polygon = new QGraphicsPolygonItem();
   this->packingView->addItem(this->polygon);
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
    auto s = std::make_shared<View::SelectionVertex>(pos);
    this->vertices.append(s);
    this->packingView->addItem(s.get());
    this->packingView->update();
    QString label = QString("%1,\t %2").arg(pos.x()).arg(pos.y());
    ui->lstVertices->addItem(label);
    ui->view->invalidateScene();
    this->redefinePolygon();

}

void ShapeSelector::sceneMousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QPointF pos = event->scenePos();
    this->addVertex(pos);
}

void ShapeSelector::resetPolygon()
{
    this->vertices.clear();
    ui->lstVertices->clear();
    this->selectedRow = -1;
}

void ShapeSelector::readShapeFile()
{
    QString filename;
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("Circle Packing Shape Files (*.cpshape)");
    dialog.setViewMode(QFileDialog::Detail);
    if(dialog.exec()){
        filename = dialog.selectedFiles().first();
        Loader::CPShape cpshape(filename);
        this->circleResize(cpshape.getRadius());
        this->resetPolygon();
        for(QPointF p: cpshape.getVertexList()){
            this->addVertex(p);
        }
    }
}

void ShapeSelector::writeShapeFile()
{
    QString filename;
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter("Circle Packing Shape Files (*.cpshape)");
    dialog.setViewMode(QFileDialog::Detail);
    if(dialog.exec()){
        filename = dialog.selectedFiles().first();
        QList<QPointF> points;
        for(auto s: this->vertices){
            points.append(QPointF(s->pos()));
        }
        Loader::CPShape cpshapefile(points, this->circleRadius);
        cpshapefile.writeFile(filename);
    }
}

void ShapeSelector::resizeEvent(QResizeEvent *event)
{
    ui->view->resetMatrix();
    Q_UNUSED(event);
    qreal xscale = ui->view->size().width();
    qreal yscale = ui->view->size().height();
    ui->view->scale(xscale*0.5, yscale*0.5);
    //ui->view->scale(xscale*0.0, yscale*0.0);
}

QPointF ShapeSelector::getCenter()
{
    QPointF sum;
    for(auto a: this->vertices) sum += a->pos();
    sum /= this->vertices.length();
    qDebug() << "Got center at " << sum;
    return sum;
}

void ShapeSelector::setupPolygon()
{
    this->polygon = new QGraphicsPolygonItem();
    this->packingView->addItem(this->polygon);
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

void ShapeSelector::redefinePolygon()
{
    //if we have at least three vertices, then we can now also allow culling
    if(this->vertices.length() >= 3){
        qDebug() << "showing the polygon";
        ui->btnCull->setEnabled(true);
        //construct a QPolygon using the vertices
        QPolygonF p;
        for(auto v: this->vertices) p.append(v->pos());
        p.append(this->vertices.first()->pos());
        this->polygon->setPolygon(p);
        this->polygon->setZValue(this->packingView->items().first()->zValue() + 1);
        this->polygon->setVisible(true);
    }
    else{
        this->polygon->setVisible(false);
    }
}


