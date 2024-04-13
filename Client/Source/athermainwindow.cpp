#include "athermainwindow.hpp"
#include "./ui_athermainwindow.h"
#include "global.hpp"
#include "login.hpp"
#include "interacttab.hpp"
#include "listener.hpp"
#include <QTimer>
#include "generatepayload.h"
#include "c2config.h"
#include "about.h"
#include "QDesktopWidget"
#include <QDesktopServices>
#include <QUrl>
auto ListenerTabName = "Listener";

void AtherMainWindow::updateCounter()
{
        for (int row = 0; row < ui->TableSessions->rowCount(); ++row)
        {
            auto lastping = ui->TableSessions->item(row, 6)->text();
            auto Target = TargetInfoMap[ui->TableSessions->item(row, 0)->text()];
            if (Target)
            {
                if(Target->TargetData){
                    Target->TargetData->LastPing1970++;
                    // Calculate days, hours, minutes, and seconds
                    int seconds = Target->TargetData->LastPing1970 % 60;
                    int minutes = (Target->TargetData->LastPing1970 / 60) % 60;
                    int hours = (Target->TargetData->LastPing1970 / 3600) % 24;
                    int days = Target->TargetData->LastPing1970 / 86400;

                    // Create a formatted string
                    QString formattedTime;
                    if (days > 0)
                    {
                        formattedTime = QString("%1d %2h").arg(days).arg(hours);
                    }
                    else if (hours > 0)
                    {
                        formattedTime = QString("%1h %2m").arg(hours).arg(minutes);
                    }
                    else if (minutes > 0)
                    {
                        formattedTime = QString("%1m %2s").arg(minutes).arg(seconds);
                    }
                    else
                    {
                        formattedTime = QString("%1s").arg(seconds);
                    }

                    // Update the QTableWidgetItem with the formatted time
                    QTableWidgetItem *newItem = new QTableWidgetItem(Target->TargetData->LastPing + " | " + formattedTime);
                    newItem->setTextAlignment(Qt::AlignCenter);
                    ui->TableSessions->setItem(row, 6, newItem);
                }
            }
        }

}


void AtherMainWindow::SetTeamServerTabName()
{

    this->ui->TeamServerTab->setTabText(0,Teamserver->Host+":"+Teamserver->Port);

}


AtherMainWindow::AtherMainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::AtherMainWindow)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCounter())); // Using SIGNAL and SLOT
    timer->start(1000);                                             // Update every 1000ms (1 second)
    ui->setupUi(this);
    this->ListenerTab=new Listener(new QWidget);
    AddBottomTab(this->ListenerTab, ListenerTabName);
    QDesktopWidget *desktop = QApplication::desktop();

    int screenWidth, width;
    int screenHeight, height;
    int x, y;
    QSize windowSize;

    screenWidth = desktop->width();
    screenHeight = desktop->height(); // get height of screen

    windowSize = size(); // size of our application window
    width = windowSize.width();
    height = windowSize.height();

    // little computations
    x = (screenWidth - width) / 2;
    y = (screenHeight - height) / 2;
    y -= 50;

    // move window to desired coordinates
    move ( x, y );
}

AtherMainWindow::~AtherMainWindow()
{
    delete ui;
}

void AtherMainWindow::AtherMainWindow::Initializer()
{

    this->setVisible(false);
    qDebug("this->setVisible(false);");
    UILogin->show();

    qDebug(" UIWareedLogin->show();");
}

void AtherMainWindow::AddNewCell(int row, int column, QString item)
{

    auto NewItem = new QTableWidgetItem(item);
    NewItem->setTextAlignment(Qt::AlignCenter);
    this->ui->TableSessions->setItem(row, column, NewItem);
}



void AtherMainWindow::AddNewListenerInfo(ListenerInfo *NewListener){
    qDebug("Add new listener");
    this->ListenerTab->AddNewListener(NewListener);

}

void AtherMainWindow::DelListenerInfo(ListenerInfo *DelListener){
    qDebug("Del listener");
    this->ListenerTab->DelListener(DelListener);

}

void AtherMainWindow::UpdateListenerHits(ListenerInfo *HListener,QString Hits){
    qDebug("UpdateListenerHits");
    this->ListenerTab->UpdateListenerHits(HListener,Hits);


}


void AtherMainWindow::CheckSessionTable(TargetInfo *NewTarget)
{
    for (int irow = 0; irow < ui->TableSessions->rowCount(); ++irow)
    {
        if (ui->TableSessions->item(irow, 0)->text() == NewTarget->TargetData->ID)
        {
            return;
        }
     }
        int Row = this->ui->TableSessions->rowCount();
        this->ui->TableSessions->insertRow(Row);
        qDebug("Add NewTarget To Table");
        AddNewCell(Row, 0, NewTarget->TargetData->ID);
        AddNewCell(Row, 1, NewTarget->TargetData->Computer);
        AddNewCell(Row, 2, NewTarget->TargetData->UserName);
        AddNewCell(Row, 3, NewTarget->TargetData->PID);
        AddNewCell(Row, 4, NewTarget->TargetData->OS);
        AddNewCell(Row, 5, NewTarget->TargetData->Privilege);
        AddNewCell(Row, 6, "0");

}


void AtherMainWindow::AddNewSession(TargetSession *NewTarget)
{
    // QVariant(NewTarget->PID).toString()
    auto Target = TargetInfoMap[NewTarget->ID];
    int Row = -1;
    if (!Target)
    {
        Target= new TargetInfo();
        Target->UITabInteract = new interactTab(new QWidget);
        Target->TargetData=NewTarget;
        Target->UITabInteract->Target = Target;
        qDebug("NewTarget->UITabInteract->Target=NewTarget;");
        Target->UITabInteract->SetDispatchInfo(Target);
        TargetInfoMap.insert(Target->TargetData->ID, Target);
        Row = this->ui->TableSessions->rowCount();
        this->ui->TableSessions->insertRow(Row);
        qDebug("NewTarget ADD!");
    }
    else
    {
        for (int irow = 0; irow < ui->TableSessions->rowCount(); ++irow)
        {
            if (ui->TableSessions->item(irow, 0)->text() == Target->TargetData->ID)
            {
                Row = irow;
                Target->TargetData = NewTarget;
                break;
            }
        }
    }

    if (Row != -1)
    {
        qDebug("Add NewTarget To Table");
        AddNewCell(Row, 0, NewTarget->ID);
        AddNewCell(Row, 1, NewTarget->Computer);
        AddNewCell(Row, 2, NewTarget->UserName);
        AddNewCell(Row, 3, NewTarget->PID);
        AddNewCell(Row, 4, NewTarget->OS);
        AddNewCell(Row, 5, NewTarget->Privilege);
        AddNewCell(Row, 6, "0");
        Target->UITabInteract->SetLableInfo("["+NewTarget->ID+" | "+NewTarget->Computer+"]"+" - "+NewTarget->UserName);
    }
}

void AtherMainWindow::AddBottomTab(QWidget *page, const QString &label)
{

    for (int i = 0; i < this->ui->TabWidgetBottom->count(); i++)
    {
        if (this->ui->TabWidgetBottom->tabText(i) == label)
        {
            this->ui->TabWidgetBottom->setCurrentIndex(i);
            return;
        }
    }

    // int position = this->ui->TabWidgetBottom->count() - 1;
    int position = this->ui->TabWidgetBottom->count();
    this->ui->TabWidgetBottom->addTab(page, label);
    this->ui->TabWidgetBottom->setCurrentIndex(position);
}
void AtherMainWindow::RigthClickSessionTableH(const QPoint &pos)
{

    if (!ui->TableSessions->itemAt(pos))
        return;

    auto MenuStyle = QString(
        "QMenu {"
        "    background-color: #282a36;"
        "    color: #f8f8f2;"
        "    border: 1px solid #44475a;"
        "}"
        "QMenu::separator {"
        "    background: #44475a;"
        "}"
        "QMenu::item:selected {"
        "    background: #44475a;"
        "}"
        "QAction {"
        "    background-color: #282a36;"
        "    color: #f8f8f2;"
        "}");

    auto SessionID = ui->TableSessions->item(ui->TableSessions->currentRow(), 0)->text();
    auto TargetInfo = TargetInfoMap[SessionID];
    qDebug("SessionID RigthClick [%s]", SessionID.toStdString().c_str());

    auto seperator = new QAction();
    auto seperator2 = new QAction();
    auto seperator3 = new QAction();
    auto seperator4 = new QAction();

    seperator->setSeparator(true);
    seperator2->setSeparator(true);
    seperator3->setSeparator(true);
    seperator4->setSeparator(true);


    QMenu SessionMenu;


    SessionMenu.addAction("Interact");
    SessionMenu.addAction(seperator);
    SessionMenu.addAction(seperator3);
    SessionMenu.addAction("Remove");
    SessionMenu.setStyleSheet(MenuStyle);
    auto *action = SessionMenu.exec(ui->TableSessions->horizontalHeader()->viewport()->mapToGlobal(pos));
    if (action)

    {
        if (action->text().compare("Interact") == 0)
        {
            qDebug("interact clicked on Session [%s]", SessionID.toStdString().c_str());
            auto tabName = "[" + TargetInfo->TargetData->ID + "] " + TargetInfo->TargetData->UserName + "\\" + TargetInfo->TargetData->Computer;
            AddBottomTab(TargetInfo->UITabInteract, tabName);

            TargetInfo->UITabInteract->setfoc();
        }else if(action->text().compare("Remove") == 0){
            ui->TableSessions->removeRow(ui->TableSessions->currentRow());
        }
    }
    delete seperator;
    delete seperator2;
    delete seperator3;
    delete seperator4;
}

void AtherMainWindow::on_TableSessions_customContextMenuRequested(const QPoint &pos)
{
    RigthClickSessionTableH(pos);
}

void AtherMainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "WareedC2",
                                                               tr("Are you sure?\n"),
                                                               QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                               QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes)
    {
        event->ignore();
        return;
    }
    else
    {
        event->accept();
        AtherApp->exit();
    }
}

void AtherMainWindow::on_TabWidgetBottom_tabCloseRequested(int index)
{
    this->ui->TabWidgetBottom->removeTab(index);
}

void AtherMainWindow::on_actionActiveDomain_triggered()
{
    qDebug("Icon chicked");
    AddBottomTab(this->ListenerTab, ListenerTabName);
}


void AtherMainWindow::on_actionwindows_triggered()
{
    UIGeneratePayload->show();
}


void AtherMainWindow::on_actionexit_triggered()
{
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "WareedC2",
                                                               tr("Are you sure?\n"),
                                                               QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                               QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes)
    {
        return;
    }
    else
    {
        AtherApp->exit();
    }
}


void AtherMainWindow::on_actionConfigration_triggered()
{
 UIC2Config->show();
}


void AtherMainWindow::on_actionhelp_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/Faisal-P27/WAREED-DNS-C2"));

}


void AtherMainWindow::on_actionabout_triggered()
{
    UIAbout->show();
}

