#ifndef PFILE_HPP
#define PFILE_HPP

#include <QWidget>

#include "Node.hpp"
#include "Packing.hpp"

class PFile
{
public:
    PFile(QString filename);
    Packing *generatePacking();
private:
    QString filename;
    QList<Node*> nodes;
    PackingType packingType;

};

#endif // PFILE_HPP
