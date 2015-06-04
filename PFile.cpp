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

    QStringList words = in.readAll().split(QRegExp("\\s"), QString::SkipEmptyParts);
    f.close();
    qDebug() << words;
    //now we have to find the index of the markers.
    int num_nodes = words.at(words.indexOf(QRegExp("NODECOUNT:"))+1).toInt();
    qDebug() << "Number of nodes: " << num_nodes;
    int radii_base = words.indexOf(QRegExp("RADII:")) + 1;
    qDebug() << "radii_base: " << radii_base;
    int center_base = words.indexOf(QRegExp("CENTERS:")) + 1;
    qDebug() << "center_base" << center_base;
    //now construct the nodes.
    for(int i = 0; i < num_nodes; i++){
        qreal radius = words.at(radii_base + i).toDouble() * 100;
        qreal x = words.at(center_base + 2*i).toDouble() * 100;
        qreal y = words.at(center_base + 2*i + 1).toDouble()* 100;
        qDebug() << QString("New Node at %1, %2 with r=%3").arg(x).arg(y).arg(radius);
        nodes.append(new Node(i+1, QPointF(x, y), radius));
    }
}

Packing* PFile::generatePacking()
{
    Packing *p = new Packing(Packing::PackingType::EuclideanPacking);
    for(Node *n: nodes) p->addNode(n);
    return p;
}

