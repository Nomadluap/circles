#ifndef EUCLIDEANPACKING_HPP
#define EUCLIDEANPACKING_HPP

#include "Packing.hpp"
#include "Node.hpp"
#include <QWidget>

class EuclideanPacking : public Packing
{
    Q_OBJECT
public:
    EuclideanPacking();

protected:
    void addCircle(Node *n) Q_DECL_OVERRIDE;
};

#endif // EUCLIDEANPACKING_HPP
