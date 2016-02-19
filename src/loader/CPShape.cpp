#include "CPShape.hpp"
#include <memory>
#include <QFile>
#include <QTextStream>
#include <QStringList>

using namespace Circles;
using namespace Circles::Loader;

CPShape::CPShape()
{

}

CPShape::CPShape(const CPShape& other):
    vertices(other.vertices),
    radius(other.radius)
{

}

CPShape::CPShape(CPShape&& other):
    vertices(std::move(other.vertices)),
    radius(other.radius)
{

}

CPShape& CPShape::operator=(const CPShape& other)
{
    this->vertices = other.vertices;
    this->radius = other.radius;
    return *this;
}

CPShape& CPShape::operator=(CPShape&& other)
{
    this->vertices = std::move(other.vertices);
    this->radius = other.radius;
    return *this;
}

CPShape::CPShape(const QString& filename)
{
    QFile cpshapefile(filename);
    if(cpshapefile.open(QFile::ReadOnly)){
        QTextStream f(&cpshapefile);
        QStringList lines = f.readAll().split(QRegExp("\\n"), QString::SkipEmptyParts);

        // the first line is the circle radius
        bool ok = true;
        QString rad = lines.first();
        this->radius = rad.toDouble(&ok);
        if(!ok) this->radius = 0.100;
        //now remaining lines form the vertex list
        for(int i = 1; i < lines.length(); i++){
            //parse first, second number from line
            QString line = lines.at(i);
            //skip comment lines
            if(line.trimmed().startsWith("#")) continue;
            QString xstring = line.split(",").first();
            QString ystring = line.split(",").last();
            bool xOK = true;
            bool yOK = true;
            qreal x = xstring.toDouble(&xOK);
            qreal y = ystring.toDouble(&yOK);
            if(!xOK) x = 0.0;
            if(!yOK) y = 0.0;
            //constrain to unit square
            if(x < -1.0) x = -1.0;
            if(x > 1.0) x = 1.0;
            if(y < -1.0) y = -1.0;
            if(y > 1.0) y = 1.0;

            QPointF p(x, y);
            this->vertices.append(p);
        }
    }
}

CPShape::CPShape(const QList<QPointF>& pointlist, qreal radius):
    vertices(pointlist),
    radius(radius)
{

}

bool CPShape::writeFile(const QString& filename)
{
     QString fname(filename);
     if(!fname.endsWith(".cpshape")) fname.append(".cpshape");
     QFile shapeFile(fname);
     if(!shapeFile.open(QFile::WriteOnly)){
         return false;
     }
     QTextStream st(&shapeFile);
     st << this->radius << "\n";
     for(QPointF p: this->vertices){
         st << p.x() << ", " << p.y() << "\n";
     }
     st.flush();
     shapeFile.close();
     return true;
}

QList<QPointF> CPShape::getVertexList()
{
    return this->vertices;
}

qreal CPShape::getRadius()
{
    return this->radius;
}
