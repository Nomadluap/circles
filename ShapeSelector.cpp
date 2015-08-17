#include "ShapeSelector.hpp"
#include "ui_ShapeSelector.h"

ShapeSelector::ShapeSelector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShapeSelector)
{
    ui->setupUi(this);
}

ShapeSelector::~ShapeSelector()
{
    delete ui;
}

void ShapeSelector::manualAddVertex()
{

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

void ShapeSelector::addVertex(QPointF pos)
{

}


