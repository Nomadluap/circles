#include "ShapeSelector.hpp"
#include "ui_ShapeSelector.h"

ShapeSelector::ShapeSelector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShapeSelector)
{
    ui->setupUi(this);
}

ShapeSelector::~ShapeSelector()
{
    delete ui;
}
