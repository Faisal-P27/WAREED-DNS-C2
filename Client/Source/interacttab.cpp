#include "interacttab.hpp"
#include "ui_interacttab.h"
#include "Include/helper/colortext.hpp"
#include "Include/command/command.h"
#include "QFont"
#include "QKeyEvent"
#include "global.hpp"



const int PrintConsoleShell =1;
const int PrintConsoleSendTask =2;
const int PrintConsoleWarning =3;
const int PrintConsoleSuccessful =4;

interactTab::interactTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::interactTab)
{
    ui->setupUi(this);
    MainCommands=new command();
    QFont font("Consolas", 11);
    this->ui->interactTextEdit->setFont(font);
    for (const auto& command : CommandList) {
        CommandCompleterList.append(command.CommandString);
        if(command.CommandString!="help"){
            CommandCompleterList.append("help "+command.CommandString);
        }
    }
    ui->InteractLineEdit->installEventFilter(this);
    commandHistoryIndex = 0;
    // Create a new completer using the filtered commands
    QCompleter *completer = new QCompleter(CommandCompleterList, this);

    // Set the completion mode to PopupCompletion
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::InlineCompletion);

//    // Set the completer to the line edit
    ui->InteractLineEdit->setCompleter(completer);



}

interactTab::~interactTab()
{
    delete ui;
}
bool interactTab::eventFilter(QObject *obj, QEvent *event) {
    if (obj == ui->InteractLineEdit && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        switch (keyEvent->key()) {
            case Qt::Key_Tab:
                qDebug("tab clicked");
                handleTabKey();
                return true;
            case Qt::Key_Up:
            qDebug("Key_Up clicked");
                handleUpKey();
                return true;
            case Qt::Key_Down:
            qDebug("DownKey clicked");
                handleDownKey();
                return true;
            default:
                break;
        }
    }
    // Call base class method for unhandled events
    return QWidget::eventFilter(obj, event);
}

void interactTab::handleTabKey() {
    // Check if the line edit has a completer
    if (ui->InteractLineEdit->completer()) {
        QString completedString = ui->InteractLineEdit->completer()->currentCompletion();
        if (!completedString.isEmpty()) {
            ui->InteractLineEdit->setText(completedString);
            // move the cursor to the end of the line edit
            ui->InteractLineEdit->setCursorPosition(ui->InteractLineEdit->text().length());
        }
    }
}


void interactTab::handleUpKey() {
    // Navigate back through the command history, if possible.
    if (commandHistoryIndex > 0) {
        commandHistoryIndex--;
        ui->InteractLineEdit->setText(commandHistory.at(commandHistoryIndex));
    }

}

void interactTab::handleDownKey() {
    // Navigate forward through the command history, if there's a next item.
    if (commandHistoryIndex < commandHistory.size()) {
        // Increment first to navigate to the next command.
        commandHistoryIndex++;

        // Check if the new index points to a valid command in the history.
        if (commandHistoryIndex < commandHistory.size()) {
            ui->InteractLineEdit->setText(commandHistory.at(commandHistoryIndex));
        }
        else {
            // If the index is beyond the last command, clear the line edit.
            ui->InteractLineEdit->clear();
        }
    }
    // No else part needed here as all conditions are covered.
}

void interactTab::addCommand(const QString &command) {
    // Add the command to the history.
    commandHistory.append(command);
    // Update the index to point to the "end" of the history, ready for a new command.
    commandHistoryIndex = commandHistory.size();

}

void interactTab::SetLableInfo(const QString Text){
    ui->SessionInfoLable->setText(Text);


}


void interactTab::AppendCompleter(const QString &text)
{

    addCommand(text);
}

void interactTab::setfoc()
{
    this->ui->InteractLineEdit->setFocus();
}


void interactTab::SetDispatchInfo(TargetInfo *Target)
{

    MainCommands->Target=Target;
    MainCommands->TabInteract=Target->UITabInteract;

}

void interactTab::PrintConsole( const QString &text ) const
{

        auto TaskMessage = ColorText::Cyan( ColorText::Cyan( text.toHtmlEscaped() ));
        ui->interactTextEdit->append(TaskMessage);

}





QString interactTab::TaskInfo( bool Show, QString TaskID, const QString &text ) const
{
    if ( TaskID == nullptr ){
        TaskID = gen_random( 3 ).c_str();
    }

    if ( ! Show )
    {
        auto TaskMessage = ColorText::Cyan( "[INFO]" ) + " "+ ColorText::Comment( "[" + TaskID + "]" ) + " " + ColorText::Cyan( text.toHtmlEscaped() );
        ui->interactTextEdit->append(TaskMessage);
    }
    return TaskID;
}



void interactTab::TaskResult( bool Show, QString TaskID, const QString &text,int Type )
{



    switch (Type) {
        case PrintConsoleShell:{
            QString Result="";
            Result="Task Output:";
            auto TaskMessage = ColorText::Green( "[GOOD] " ) + ColorText::Comment( QDate::currentDate().toString( "dd/MM/yyyy" ) + " "+ QTime::currentTime().toString( "hh:mm:ss" ))+ " "+  Result ;
            ui->interactTextEdit->append(TaskMessage);
            ui->interactTextEdit->append(text);
            break;
                }
         case PrintConsoleSendTask:{
            auto Message = ColorText::Cyan( "[INFO] " ) + text  ;
            ui->interactTextEdit->append(Message);
            break;
            }
        case PrintConsoleWarning:{
           auto Message = ColorText::Yellow( "[WARN] " ) + text  ;
           ui->interactTextEdit->append(Message);
           break;}
        case PrintConsoleSuccessful:{
           auto Message = ColorText::Green( "[GOOD] " ) +   text  ;
           ui->interactTextEdit->append(Message);
           break;
           }
    }



}

QString interactTab::TaskError( const QString &text ) const
{
    auto TaskMessage = ColorText::Red( "[!]" ) + " " + text.toHtmlEscaped();
    ui->interactTextEdit->append( TaskMessage );
    return TaskMessage;
}



void interactTab::ConsoleAppend(QString Text)
{

    ui->interactTextEdit->append(Text.toHtmlEscaped());

}

void interactTab::Clear()
{

    ui->interactTextEdit->clear();

}
void interactTab::on_InteractLineEdit_returnPressed()
{
    QString text=ui->InteractLineEdit->text();
    ui->InteractLineEdit->setText("");

    QString Command= QString(ColorText::Comment( QDate::currentDate().toString( "dd/MM/yyyy" ) + " "+ QTime::currentTime().toString( "hh:mm:ss" ) +
    " [" + Teamserver->User + "] " ) +
    ColorText::UnderlineRed( "ATHER" ) + ColorText::Cyan(" » ") + text);
    ui->interactTextEdit->append(Command);
    MainCommands->DispatchCommand(false,"",text);
    if(!text.isEmpty()){
        AppendCompleter(text);
    }


}



