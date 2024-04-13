#include "about.h"
#include "ui_about.h"
#include "global.hpp"
about::about(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::about)
{
    ui->setupUi(this);
    CenterWidgets(parent);
}

about::~about()
{
    delete ui;
}

void about::on_pushButton_clicked()
{
    this->hide();

}

