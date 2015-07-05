#include "PFile.hpp"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QStringList>
#include <QRegExp>

#include <QDebug>

PFile::PFile(QString filename)
{
    this->filename = filename;
    QFile f(filename);
    //try opening
    if(!f.open(QIODevice::ReadOnly)){
        QMessageBox::information(0, "Error", f.errorString());
        return;
    }
    QTextStream in(&f);
    //split file into list of words
    QStringList words = in.readAll().split(QRegExp("\\s"), QString::SkipEmptyParts);

    f.close();

    //now we have to find the index of the markers.
    int num_nodes = words.at(words.indexOf(QRegExp("NODECOUNT:"))+1).toInt();
    qDebug() << "Number of nodes: " << num_nodes;
    int flowers_base = words.indexOf(QRegExp("FLOWERS:")) + 1;
    qDebug() << "flowers_base: " << flowers_base;
    int radii_base = words.indexOf(QRegExp("RADII:")) + 1;
    qDebug() << "radii_base: " << radii_base;
    int center_base = words.indexOf(QRegExp("CENTERS:")) + 1;
    qDebug() << "center_base" << center_base;
    int geometry_base = words.indexOf(QRegExp("GEOMETRY:"));

    if(words.at(geometry_base+1).toLower() == "hyperbolic"){
        this->packingType = PackingType::HyperbolicPacking;
    }
    else if(words.at(geometry_base+1).toLower() == "euclidean"){
        this->packingType = PackingType::EuclideanPacking;
    }
    else{
        QMessageBox::warning(nullptr, "Error", "Packing geometry is not understood");
        return;
    }

    //now construct the nodes. If the radii are defined then we pass both
    //the radii and the positions to the nodes. Otherwise, we only pass the
    //neibhour information.
    for(int i = 0; i < num_nodes; i++){
        //nodes have defined position
        if(radii_base != -1){
            qreal radius = words.at(radii_base + i).toDouble();
            qreal x = words.at(center_base + 2*i).toDouble();
            qreal y = words.at(center_base + 2*i + 1).toDouble() ;
            qDebug() << QString("New Node at %1, %2 with r=%3").arg(x).arg(y).arg(radius);
            nodes.append(new Node(i+1, QPointF(x, y), radius));
        }
        //nodes don't have defined position Generate a blank node.
        else{
            nodes.append(new Node(i+1));
            qDebug() << "New node";
        }
    }
    //now generate neibhour information from flowers data
    int prev_offset = 0; //reading offset
    for(int i = 0; i < num_nodes; i++){
        //read number of neibhours to expect
        int neibhours = words.at(flowers_base + prev_offset + 1).toInt() + 1;
        qDebug() << "Node " << i+1 << " has " << neibhours << " neibhours";
        //create all connections
        for(int n = 0; n < neibhours; n++){
            //this node connects to the following nodes
            int nodeIndex = words.at(flowers_base + prev_offset + 2 + n).toInt() - 1;
            nodes.at(i)->addNeibhour(nodes.at(nodeIndex));
            qDebug() << "neibhouring nodes: " << i+1 << " and " << nodeIndex+1;
        }
        //now advance prev_offset
        prev_offset += neibhours + 2;
    }
}

Packing* PFile::generatePacking()
{
    Packing *p = new Packing(this->packingType);
    for(Node *n: nodes){
        n->sortNeibhours();
        p->addNode_fast(n);
    }
    p->recomputeConnectors();
    return p;
}

