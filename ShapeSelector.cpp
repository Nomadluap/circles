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
