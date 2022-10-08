#include "SaveOptionString.h"
#include "ui_SaveOptionString.h"

SaveOptionString::SaveOptionString(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SaveOptionString)
{
    ui->setupUi(this);
}

SaveOptionString::~SaveOptionString()
{
    delete ui;
}
