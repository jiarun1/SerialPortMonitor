#ifndef ATMODEEDIT_H
#define ATMODEEDIT_H

#include <QWidget>
#include <Surface/ATModeEdit.h>
//#include <Middle/ATToolBox.h>

namespace Ui {
class ATModeEdit;
}

class ATModeEdit : public QWidget
{
    Q_OBJECT

public:
    explicit ATModeEdit(QWidget *parent = nullptr);
    ~ATModeEdit();

    void Init();

private slots:
    void on_toolbox_pushbutton_clicked();

private slots:
    void on_Add_PushButton_clicked();

    void on_Delete_PushButton_clicked();

    void on_Edit_PushButton_clicked();

    void on_Export_PushButton_clicked();

    void on_Import_PushButton_clicked();

private:
    Ui::ATModeEdit *ui;


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

#endif // ATMODEEDIT_H
