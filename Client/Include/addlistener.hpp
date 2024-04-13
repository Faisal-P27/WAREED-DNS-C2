#ifndef ADDLISTENER_HPP
#define ADDLISTENER_HPP

#include <QDialog>
#include "global.hpp"
namespace Ui {
class AddListener;
}

class AddListener : public QDialog
{
    Q_OBJECT

public:
    explicit AddListener(QWidget *parent = nullptr);
    ~AddListener();

    void AddCallBackDomain(QString DomainName);
private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddListener *ui;
};

#endif // ADDLISTENER_HPP
