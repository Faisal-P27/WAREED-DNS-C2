#include "Include/helper/wmessagebox.hpp"

WMessageBox::WMessageBox()
{

}
void WMessageBox::CriticalMessage(const QString &title, const QString &Text, const QString &TextInfo)
{
    QMessageBox message;
    message.setWindowTitle(title);
    message.setText(Text + ":");
    message.setInformativeText(TextInfo);
    message.setIcon(QMessageBox::Critical);
    message.setStandardButtons(QMessageBox::Ok);
    message.setDefaultButton(QMessageBox::Ok);
    message.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    message.exec();
}

void WMessageBox::GoodMessage(const QString &title, const QString &Text, const QString &TextInfo)
{

    QMessageBox message;
    message.setWindowTitle(title);
    message.setText(Text+":");
    message.setInformativeText(TextInfo);
    message.setIcon(QMessageBox::Information);
    message.setStandardButtons(QMessageBox::Ok );
    message.setDefaultButton(QMessageBox::Ok);
    message.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    message.exec();

}
