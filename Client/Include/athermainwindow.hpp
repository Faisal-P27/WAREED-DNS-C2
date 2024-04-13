#ifndef ATHERMAINWINDOW_H
#define ATHERMAINWINDOW_H

#include <QMainWindow>
#include "global.hpp"
#include <QTimer>
#include "listener.hpp"
QT_BEGIN_NAMESPACE
namespace Ui { class AtherMainWindow; }
QT_END_NAMESPACE

class AtherMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    AtherMainWindow(QWidget *parent = nullptr);
    ~AtherMainWindow();
    void Initializer();
    void AddNewSession(TargetSession *NewTarget);
    void AddNewCell(int row,int column,QString item);
    void RigthClickSessionTableH( const QPoint &pos);
    void AddBottomTab(QWidget *page,  const QString &label);
    void closeEvent (QCloseEvent *event);
    void SetTeamServerTabName();
    void AddNewListenerInfo(ListenerInfo *NewListener);
    void UpdateListenerHits(ListenerInfo *HListener, QString Hits);
    void DelListenerInfo(ListenerInfo *DelListener);
    void CheckSessionTable(TargetInfo *NewTarget);
private slots:
    void on_TableSessions_customContextMenuRequested(const QPoint &pos);
    void updateCounter();
    void on_TabWidgetBottom_tabCloseRequested(int index);
    void on_actionActiveDomain_triggered();

    void on_actionwindows_triggered();

    void on_actionexit_triggered();

    void on_actionConfigration_triggered();

    void on_actionhelp_triggered();

    void on_actionabout_triggered();

private:
    Ui::AtherMainWindow *ui;
    QTimer* timer;
    Listener* ListenerTab;

};
#endif // ATHERMAINWINDOW_H
