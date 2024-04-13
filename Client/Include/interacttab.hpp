#ifndef INTERACTTAB_H
#define INTERACTTAB_H

#include <QWidget>
#include "global.hpp"
#include "Include/command/command.h"

namespace Ui {
    class interactTab;
}

class interactTab : public QWidget {
    Q_OBJECT

public:
    explicit interactTab(QWidget *parent = nullptr);
    ~interactTab();

    // Existing functions...
    void SetLableInfo(const QString Text);
    void setfoc();
    void SetDispatchInfo(TargetInfo* Target);
    QString TaskInfo( bool Show, QString TaskID, const QString &text ) const;
    QString TaskError( const QString &text ) const;
    void TaskResult( bool Show, QString TaskID, const QString &text, int Type );
    void ConsoleAppend(QString Text);
    TargetInfo* Target;
    class command *MainCommands;
    QStringList CommandCompleterList;
    void PrintConsole(const QString &text) const;
    void AppendCompleter(const QString &text);

    // New or modified functions for event handling...
    bool eventFilter(QObject *obj, QEvent *event);
    void addCommand(const QString &command);

    void Clear();
private:
    Ui::interactTab *ui;
    QStringList commandHistory;
    int commandHistoryIndex = 0;
    void updateCompleter(const QString &text);
    void handleTabKey();
    void handleUpKey();
    void handleDownKey();

private slots:
    void on_InteractLineEdit_returnPressed();
};

#endif
