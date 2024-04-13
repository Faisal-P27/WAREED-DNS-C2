#ifndef C2CONFIG_H
#define C2CONFIG_H

#include <QDialog>
#include "global.hpp"
namespace Ui {
class C2Config;
}

class C2Config : public QDialog
{
    Q_OBJECT

public:
    explicit C2Config(QWidget *parent = nullptr);
    ~C2Config();

    void SetC2Config(const QString& MagicValue, const QStringList& Domains, const QString& DefaultDNSServer, QString& ListenerPort, int Sleep, int Jitter, int LookupDelay);
private slots:
    void on_pushButtonClose_clicked();

private:
    Ui::C2Config *ui;
};

#endif // C2CONFIG_H
