#ifndef loader_CPSHAPE_HPP
#define loader_CPSHAPE_HPP

#include <QList>
#include <QPointF>
#include <QString>

namespace Circles{
namespace Loader{
    /**
     * A CPShape file defines a starting radius and the coordinates of the vertices of a polygon, that is intended to
     * be loaded into the shapeSelector for parsing of a circle packing that approximates that shape. Interacts with
     * files of type .cpshape
     */
    class CPShape
    {
    public:
        /**
         * Generate an empty CPShape. Suitable for assigning a vector list and radius to later, for exporting to a file.
         */
        CPShape();
        CPShape(const CPShape& other);
        CPShape(CPShape&& other);
        CPShape& operator=(const CPShape& other);
        CPShape& operator=(CPShape&& other);

        CPShape(const QString& filename);
        CPShape(const QList<QPointF>& pointlist, qreal radius);

        /**
         * Write the CPShape object out to a file, using the specified filename.
         *
         * @param filename The name of the file to write. The filename extension .cpshape will be added if not present.
         * @return True if the write was a success, false otherwise.
         */
        bool writeFile(const QString& filename);

        /**
         * Return the list of vertices in the CPShape. All vertices should reside in the unit square. (ie, all
         * components should have absolute value <= 1.0;
         *
         * @return List of coordinates.
         */
        QList<QPointF> getVertexList();

        /**
         * Get the circle radius specified in the CPShape.
         * @return
         */
        qreal getRadius();
    private:
        QList<QPointF> vertices;
        qreal radius;

    }; // class CPShape

} // namespace Loader
} // namespace Circles


#endif // CPSHAPE_HPP
