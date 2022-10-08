#ifndef ATMODE_H
#define ATMODE_H

#include <QWidget>
#include <QList>
#include <QString>
#include <QStringList>

#include <QVBoxLayout>
#include <QPushButton>

#include "Surface/ATModeEdit.h"

namespace Ui {
class ATMode;
}

class ATMode : public QWidget
{
    Q_OBJECT

public:
    explicit ATMode(QWidget *parent = nullptr);
    ~ATMode();

    void Init();

private slots:
    void on_History_ListWidget_currentTextChanged(const QString &currentText);

    void on_Send_PushButton_clicked();

    void on_Clear_PushButton_clicked();

    void on_Edit_PushButton_clicked();

    void on_toolbox_pushbutton_clicked();

private:

    Ui::ATMode *ui;
    ATModeEdit ATCommandEditWindow;

private:
    typedef struct{
        QString             CommandName;
        QString             Discription;
    }Command_t;

    typedef struct{
        QString             CategoryName;
        QList<Command_t>    CommandList;
    }Command_ToolBox_t;

    QList<Command_ToolBox_t*> Command_ToolBox;

    QStringList Category_List_All;
    QStringList Command_List_All;

    void RefreshToolbox();
    void CommandListToolboxClear();
};

#endif // ATMODE_H
