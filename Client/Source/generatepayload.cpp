#include "generatepayload.h"
#include "ui_generatepayload.h"
#include "Include/command/command.h"
#include "Include/helper/wmessagebox.hpp"
#include "generatewaiting.h"
#include "QTimer"

#include "setlistener.h"



GeneratePayload::GeneratePayload(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneratePayload)
{
    ui->setupUi(this);
    CenterWidgets(parent);
}

GeneratePayload::~GeneratePayload()
{
    delete ui;
}
int NewPayload=123;
#include <QScreen>
#include <QWidget>
#include <QGuiApplication>



void GeneratePayload::on_pushButtonGenerate_clicked()
{
    QString Type = ui->comboBoxType->currentText();
    QString Path = ui->lineEditOutputFolder->text();
    QString Name = ui->lineEditFileName->text();

    if(Name.isEmpty()){
        WMessageBox::CriticalMessage("New Payload", "Please enter a name for the new payload file.", "Error: File Name Required");
        return;
    }
    if(Path.isEmpty()){
        WMessageBox::CriticalMessage("New Payload", "Please specify an output path where the new payload file will be saved.", "Error: Output Path Required");
        return;
    }

    QJsonArray ListenersJsonArray;
    for(int i = 0; i < ui->tableWidgetListener->rowCount(); ++i){
        QTableWidgetItem* item = ui->tableWidgetListener->item(i, 0);
        if(item != nullptr && !item->text().isEmpty()){
            ListenersJsonArray.push_back(item->text());
        }
    }

    // If there are no listeners, display a message and return
    if(ListenersJsonArray.isEmpty()){
        WMessageBox::CriticalMessage("New Payload", "No listeners added. Please add at least one listener.", "Error: No Listeners Added");
        return;
    }

    QString OS = "windows";
    QJsonObject Info;
    Info.insert("OS", OS);
    Info.insert("Type", Type);
    Info.insert("Path", Path);
    Info.insert("Name", Name);
    Info.insert("Listeners", ListenersJsonArray); // Using QJsonArray for Listeners


       if(Type=="dll"){
           QString DLLType = ui->comboBoxTypeDLL->currentText();
           Info.insert("DLLType", DLLType);
           if(DLLType=="EXPORT_FUNCTION"){
                QString ExportFunction = ui->lineEditFunctionName->text();
                if(ExportFunction.isEmpty()){
                    WMessageBox::CriticalMessage("New Payload", "Please enter a function name", "Error: export function name required");
                    return;
                }
                Info.insert("ExportFunction", ExportFunction);
           }

       }
    Body_t Body = {0};
    Body.SubEvent = NewPayload;

    Body.Info = Info;
    UIGenerateWaiting->show();
    NewPackageCommand(Teamserver->User, Body);


}

void GeneratePayload::CloseWindow()
{
    this->close();
    ui->lineEditFileName->clear();
    ui->lineEditOutputFolder->clear();

}

void GeneratePayload::on_pushButtonBrowse_clicked()
{

    QString selectedFolder = QFileDialog::getExistingDirectory(this, tr("Select Output Folder"), QDir::currentPath());

    if (!selectedFolder.isEmpty()) {
        ui->lineEditOutputFolder->setText(selectedFolder);
    }
}




void GeneratePayload::on_pushButtonDelListener_clicked()
{
    // Get the index of the current row
    int currentRow = ui->tableWidgetListener->currentRow();

    // Check if a row is currently selected
    if (currentRow != -1) {
        // Remove the current row
        ui->tableWidgetListener->removeRow(currentRow);
    }
}


void GeneratePayload::on_pushButtonAddListener_clicked()
{

       // UISetListener->show();
        UISetListener->ShowSetListener();

}
void GeneratePayload::AddToListenerTable(const QString  SelectedListener)
{
    int numRows = ui->tableWidgetListener->rowCount();

    // Check if the SelectedListener is already in the table.
    bool listenerFound = false;
    for (int row = 0; row < numRows; ++row)
    {
        QTableWidgetItem *item = ui->tableWidgetListener->item(row, 0);
        if (item && item->text() == SelectedListener)
        {
            listenerFound = true;
            break;
        }
    }

    // If the listener was not found, add it to the table.
    if (!listenerFound)
    {
        // Insert a new row at the end of the table.
        ui->tableWidgetListener->insertRow(numRows);

        // Create a new QTableWidgetItem with the selected listener.
        QTableWidgetItem *newItem = new QTableWidgetItem(SelectedListener);

        // Set the item in the first column of the new row.
        ui->tableWidgetListener->setItem(numRows, 0, newItem);
    }
}


void GeneratePayload::on_comboBoxType_currentIndexChanged(const QString &arg1)
{
    if(arg1=="dll"){
        ui->widgetDLL->show();
    }
    if(arg1=="exe"){
       ui->widgetDLL->hide();
    }
}


void GeneratePayload::on_comboBoxTypeDLL_currentIndexChanged(const QString &arg1)
{
    if(arg1=="DLL_PROCESS_ATTACH"){
        ui->lineEditFunctionName->clear();
        ui->lineEditFunctionName->setReadOnly(1);

    }if(arg1=="EXPORT_FUNCTION"){
        ui->lineEditFunctionName->clear();
        ui->lineEditFunctionName->setReadOnly(0);
    }
}

