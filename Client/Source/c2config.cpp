#include "c2config.h"
#include "ui_c2config.h"

C2Config::C2Config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::C2Config)
{
    ui->setupUi(this);
    CenterWidgets(parent);
}

C2Config::~C2Config()
{
    delete ui;
}



void C2Config::SetC2Config(const QString& MagicValue, const QStringList& Domains, const QString& DefaultDNSServer, QString& ListenerPort, int Sleep, int Jitter, int LookupDelay)
{
    // Setting text fields
    ui->lineEditMagicValue->setText(MagicValue);
    ui->lineEditDefaultDNS->setText(DefaultDNSServer);
    ui->lineEditListenerPort->setText(ListenerPort);
    ui->lineEditSleep->setText(QString::number(Sleep));
    ui->lineEditJitter->setText(QString::number(Jitter));
    ui->lineEditLookupDelay->setText(QString::number(LookupDelay));

    // Clearing the table widget before filling it
    ui->tableWidgetDomains->clear();
    ui->tableWidgetDomains->setRowCount(Domains.size());
    ui->tableWidgetDomains->setColumnCount(1); // Assuming only one column for domain names

    // Setting column header for domains table
    QStringList headers;
    headers << "Domains";
    ui->tableWidgetDomains->setHorizontalHeaderLabels(headers);

    // Filling the table widget with domain names
    for (int i = 0; i < Domains.size(); ++i) {
        QTableWidgetItem* item = new QTableWidgetItem(Domains.at(i));
        ui->tableWidgetDomains->setItem(i, 0, item);
    }
}


void C2Config::on_pushButtonClose_clicked()
{
    this->hide();
}

