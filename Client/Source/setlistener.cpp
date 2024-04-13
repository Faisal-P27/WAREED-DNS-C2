#include "setlistener.h"
#include "ui_setlistener.h"
#include "generatepayload.h"
SetListener::SetListener(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetListener)
{
    ui->setupUi(this);
    CenterWidgets(parent);

}

SetListener::~SetListener()
{
    delete ui;
}



void SetListener::ShowSetListener()
{
    this->show();
    // Clear the combo box before adding new items
    ui->comboBox->clear();

    // Iterate over all the items in the ListenerMap
    QMap<QString, ListenerInfo*>::iterator it;
    for (it = ListenerMap.begin(); it != ListenerMap.end(); ++it) {
        // Use the domain name as the item text
        ui->comboBox->addItem(it.key());
    }

}


void SetListener::on_pushButton_2_clicked()
{
    QString SelectedListener = ui->comboBox->currentText();
    // Check if a listener is selected.
    if (!SelectedListener.isEmpty()) {

        UIGeneratePayload->AddToListenerTable(SelectedListener);

    }
    this->close();
}


void SetListener::on_pushButton_clicked()
{
    this->close();
}

