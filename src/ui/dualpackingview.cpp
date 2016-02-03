#include "dualpackingview.hpp"
#include "ui_dualpackingview.h"

DualPackingView::DualPackingView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DualPackingView)
{
    ui->setupUi(this);
}

DualPackingView::~DualPackingView()
{
    delete ui;
}
