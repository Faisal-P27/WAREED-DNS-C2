#include "listener.hpp"
#include "ui_listener.h"
#include "global.hpp"
#include "addlistener.hpp"
#include "Include/command/command.h"
Listener::Listener(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Listener)
{
    ui->setupUi(this);
}


int RemoveListener=121;
Listener::~Listener()
{
    delete ui;
}




void Listener::AddNewCell(int row, int column, QString item)
{

    auto NewItem = new QTableWidgetItem(item);
    NewItem->setTextAlignment(Qt::AlignCenter);
    this->ui->ListenerTableWidget->setItem(row, column, NewItem);
}



void Listener::AddNewListener(ListenerInfo * NewListener){
    qDebug("in AddNewListener");
     ListenerMap.insert(NewListener->DomainName, NewListener);
    int row=ui->ListenerTableWidget->rowCount();
    this->ui->ListenerTableWidget->insertRow(row);
    AddNewCell(row,0,NewListener->DomainName);
    AddNewCell(row,1,NewListener->Protocol);
    AddNewCell(row,2,NewListener->BindIP);
    AddNewCell(row,3,NewListener->BindPort);
    AddNewCell(row,4,NewListener->Hits);
    AddNewCell(row,5,NewListener->Status);
     qDebug("Done");
}
void Listener::DelListener(ListenerInfo *DelListener){

    for (int row = 0; row < ui->ListenerTableWidget->rowCount(); ++row)
    {
        if (ui->ListenerTableWidget->item(row, 0)->text() == DelListener->DomainName)
        {
            ui->ListenerTableWidget->removeRow(row);
            ListenerMap.remove(DelListener->DomainName);
            break;
        }

    }

}
void Listener::UpdateListenerHits(ListenerInfo *HListener,QString Hits){
    HListener->Hits=Hits;
    for (int row = 0; row < ui->ListenerTableWidget->rowCount(); ++row)
    {
        if (ui->ListenerTableWidget->item(row, 0)->text() == HListener->DomainName)
        {
            AddNewCell(row,4, HListener->Hits);
             break;
        }

    }




}

void Listener::on_pushButton_AddListener_clicked()
{
    UIAddListener->show();
}


void Listener::on_pushButton_RemoveListener_clicked()
{
    QString DomainName = ui->ListenerTableWidget->item(ui->ListenerTableWidget->currentRow(), 0)->text();

    if (!DomainName.isEmpty()) {
        QJsonObject Info;
        Info.insert("DomainName", DomainName);

        Body_t Body;
        Body.SubEvent = RemoveListener;
        Body.Info = Info;

        NewPackageCommand(Teamserver->User, Body);
    } else {
        qDebug("no listener selected");
    }
}

