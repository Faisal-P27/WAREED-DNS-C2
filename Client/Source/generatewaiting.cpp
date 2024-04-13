#include "generatewaiting.h"
#include "ui_generatewaiting.h"
#include "global.hpp"
GenerateWaiting::GenerateWaiting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenerateWaiting)
{
    ui->setupUi(this);
    CenterWidgets(parent);
}

GenerateWaiting::~GenerateWaiting()
{
    delete ui;
}
