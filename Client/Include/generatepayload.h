#ifndef GENERATEPAYLOAD_H
#define GENERATEPAYLOAD_H

#include <QDialog>
#include "global.hpp"
namespace Ui {
class GeneratePayload;
}

class GeneratePayload : public QDialog
{
    Q_OBJECT

public:
    explicit GeneratePayload(QWidget *parent = nullptr);
    ~GeneratePayload();

    void AddToListenerTable(QString SelectedListener);
    void CloseWindow();
    void AddToListenerTableDLL(const QString SelectedListener);
private slots:
    void on_pushButtonGenerate_clicked();

    void on_pushButtonBrowse_clicked();

    void on_pushButtonDelListener_clicked();

    void on_pushButtonAddListener_clicked();

    void on_comboBoxType_currentIndexChanged(const QString &arg1);

    void on_comboBoxTypeDLL_currentIndexChanged(const QString &arg1);

private:
    Ui::GeneratePayload *ui;
};

#endif // GENERATEPAYLOAD_H
