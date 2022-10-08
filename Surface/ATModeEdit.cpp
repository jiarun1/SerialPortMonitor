#include "ATModeEdit.h"
#include <QDebug>
#include "ui_ATModeEdit.h"
#include "Setting.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QFile>

ATModeEdit::ATModeEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ATModeEdit)
{
    ui->setupUi(this);

    this->setWindowTitle("AT Command Edit");
}

ATModeEdit::~ATModeEdit()
{
    delete ui;
}

void ATModeEdit::Init()
{
    RefreshToolbox();
}

void ATModeEdit::on_Add_PushButton_clicked()
{
    QString command = QString("AT+").append(ui->Command_lineEdit->text());
    QString category = ui->Categoty_LineEdit->text();
    QString discription = ui->Description_TextEdit->toPlainText();
    QToolBox* toolbox = ui->Command_ToolBox;

    Category_List_All = Settings.getATCommandCategorys();

    QWidget* category_widget_tmp = nullptr;
    Command_ToolBox_t *command_toolbox_tmp;

    int loop_num;
    for(loop_num = 0; loop_num < Category_List_All.count(); loop_num++)
    {
        if(Command_ToolBox[loop_num]->CategoryName == category)
        {
            command_toolbox_tmp = Command_ToolBox[loop_num];

            if(toolbox != nullptr)
            {
                for(int i = 0; i < toolbox->count(); i++)
                {
                    if(toolbox->itemText(i) == category)
                    {
                        category_widget_tmp = toolbox->widget(i);
                    }
                }

            }
            break;
        }
    }

    if(loop_num >= Category_List_All.count())
    {
        command_toolbox_tmp = new Command_ToolBox_t;
        command_toolbox_tmp->CategoryName = category;
        command_toolbox_tmp->CommandList.clear();
        if(toolbox != NULL)
        {
            category_widget_tmp = new QWidget(toolbox);
            toolbox->addItem(category_widget_tmp,category);
        }
    }

    if( Settings.getATCommandList(category).contains(command) )
    {
        return;
    }

    Command_t Command_List_tmp;

    Command_List_tmp.CommandName = command;
    Command_List_tmp.Discription = discription;


    command_toolbox_tmp->CommandList.append(Command_List_tmp);

    Command_ToolBox.append(command_toolbox_tmp);

    Settings.addATCommand(command,category,discription);

    if((toolbox != NULL)&&(category_widget_tmp != nullptr))
    {

        QPushButton* command_pushbutton_tmp = new QPushButton(category_widget_tmp);
        delete category_widget_tmp->layout();
        QVBoxLayout* pushbutton_boxlyout_tmp = new QVBoxLayout;

        QObjectList exist_pushbutton_list = category_widget_tmp->children();
        for(int i = 0; i < exist_pushbutton_list.count();i++)
        {
            if(exist_pushbutton_list[i]->metaObject()->className() == QString("QPushButton"))
            {
                QPushButton* exist_pushbutton_tmp = qobject_cast<QPushButton*>(exist_pushbutton_list[i]);
                pushbutton_boxlyout_tmp->addWidget(exist_pushbutton_tmp);
                connect(exist_pushbutton_tmp,SIGNAL(clicked()),this,SLOT(on_toolbox_pushbutton_clicked()));
            }
        }

        command_pushbutton_tmp->setText(command);

        pushbutton_boxlyout_tmp->addWidget(command_pushbutton_tmp);

        category_widget_tmp->setLayout(pushbutton_boxlyout_tmp);
    }

}


void ATModeEdit::on_Delete_PushButton_clicked()
{
    QString command = QString("AT+").append(ui->Command_lineEdit->text());
    QString category = ui->Categoty_LineEdit->text();
    QToolBox* toolbox = ui->Command_ToolBox;

    Category_List_All = Settings.getATCommandCategorys();

    QWidget* category_widget_tmp = nullptr;
    Command_ToolBox_t *command_toolbox_tmp;

    int loop_num;
    for(loop_num = 0; loop_num < Category_List_All.count(); loop_num++)
    {
        if(Command_ToolBox[loop_num]->CategoryName == category)
        {
            command_toolbox_tmp = Command_ToolBox[loop_num];

            if(toolbox != nullptr)
            {

                for(int i = 0; i < toolbox->count(); i++)
                {
                    if(toolbox->itemText(i) == category)
                    {
                        category_widget_tmp = toolbox->widget(i);
                    }
                }

            }
            break;
        }
    }

    if(loop_num >= Category_List_All.count())
    {
        return;
    }

    int command_list_num = command_toolbox_tmp->CommandList.count();
    for(loop_num = 0; loop_num < command_list_num; loop_num++)
    {
        if(command_toolbox_tmp->CommandList[loop_num].CommandName == command)
        {
            command_toolbox_tmp->CommandList.removeAt(loop_num);
            break;
        }
    }

    if(loop_num == command_list_num)
    {
        return;
    }

    if(command_toolbox_tmp->CommandList.count() == 0)
    {
        for(loop_num = 0; loop_num < Category_List_All.count(); loop_num++)
        {
            if(Command_ToolBox[loop_num]->CategoryName == category)
            {
                Command_ToolBox.removeAt(loop_num);
            }
        }
    }

    Settings.deleteATCommand(command,category);


    if((toolbox != nullptr) && (category_widget_tmp != nullptr))
    {
        delete category_widget_tmp->layout();
        QVBoxLayout* pushbutton_boxlyout_tmp = new QVBoxLayout;

        QObjectList exist_pushbutton_list = category_widget_tmp->children();
        int number_of_pushbutton = exist_pushbutton_list.count();
        for(int i = 0; i < exist_pushbutton_list.count();i++)
        {
            if(exist_pushbutton_list[i]->metaObject()->className() == QString("QPushButton"))
            {
                QPushButton* exist_pushbutton_tmp = qobject_cast<QPushButton*>(exist_pushbutton_list[i]);

                if(exist_pushbutton_tmp->text().toStdString() == command.toStdString())
                {
                    disconnect(exist_pushbutton_tmp,SIGNAL(clicked()),this,SLOT(on_toolbox_pushbutton_clicked()));
                    exist_pushbutton_tmp->deleteLater();
                    number_of_pushbutton--;
                    continue;
                }
                pushbutton_boxlyout_tmp->addWidget(exist_pushbutton_tmp);
            }
        }

        if(number_of_pushbutton == 0)
        {
            category_widget_tmp->deleteLater();
        } else {
            category_widget_tmp->setLayout(pushbutton_boxlyout_tmp);
        }
    }


    return;

}


void ATModeEdit::on_Edit_PushButton_clicked()
{
    QString command = QString("AT+").append(ui->Command_lineEdit->text());
    QString category = ui->Categoty_LineEdit->text();
    QString discription = ui->Description_TextEdit->toPlainText();
    QToolBox* toolbox = ui->Command_ToolBox;

    Category_List_All = Settings.getATCommandCategorys();

    QWidget* category_widget_tmp = nullptr;
    Command_ToolBox_t *command_toolbox_tmp;

    int loop_num;
    for(loop_num = 0; loop_num < Category_List_All.count(); loop_num++)
    {
        if(Command_ToolBox[loop_num]->CategoryName == category)
        {
            command_toolbox_tmp = Command_ToolBox[loop_num];
            break;
        }
    }

    if(loop_num >= Category_List_All.count())
    {
        return;
    }

    int command_list_num = command_toolbox_tmp->CommandList.count();
    for(loop_num = 0; loop_num < command_list_num; loop_num++)
    {
        if(command_toolbox_tmp->CommandList[loop_num].CommandName == command)
        {
            Settings.addATCommand(command,category,discription);
            break;
        }
    }

    if(loop_num == command_list_num)
    {
        return;
    }

    RefreshToolbox();
//    on_Delete_PushButton_clicked();
//    on_Add_PushButton_clicked();

}


void ATModeEdit::RefreshToolbox()
{
    QToolBox* toolbox = ui->Command_ToolBox;

    QStringList all_keys_tmp;

    Category_List_All = Settings.getATCommandCategorys();

    Command_List_All.clear();
    Command_List_All = Settings.getATCommandList();

    CommandListToolboxClear();
    Category_List_All = Settings.getATCommandCategorys();

    if(toolbox != nullptr)
    {
        for(int i = toolbox->count() - 1; i >= 0; i--)
        {
            toolbox->removeItem(i);
        }
    }

    for(int i = 0; i < Category_List_All.count();i++)
    {
        Command_ToolBox_t* command_toolbox_tmp = new Command_ToolBox_t;
        command_toolbox_tmp->CategoryName = Category_List_All.at(i);

        QWidget* category_widget_tmp = nullptr;
        QVBoxLayout* pushbutton_layout = new QVBoxLayout;
        if(toolbox != nullptr)
        {
            category_widget_tmp = new QWidget(toolbox);
            toolbox->addItem(category_widget_tmp,command_toolbox_tmp->CategoryName);
        }

        all_keys_tmp = Settings.getATCommandList(Category_List_All.at(i));

        for(int j = 0 ; j < all_keys_tmp.count(); j++)
        {
            Command_t Command_List_tmp;
            Command_List_tmp.CommandName = all_keys_tmp.at(j);
            Command_List_tmp.Discription = Settings.getATCommandDiscription(Command_List_tmp.CommandName,command_toolbox_tmp->CategoryName);

            if(toolbox != nullptr)
            {
                QPushButton* command_pushbutton_tmp = new QPushButton(category_widget_tmp);
                command_pushbutton_tmp->setText(Command_List_tmp.CommandName);
                connect(command_pushbutton_tmp,SIGNAL(clicked()),this,SLOT(on_toolbox_pushbutton_clicked()));

                pushbutton_layout->addWidget(command_pushbutton_tmp);
            }

            command_toolbox_tmp->CommandList.append(Command_List_tmp);
        }

        if(toolbox != nullptr)
        {
            category_widget_tmp->setLayout(pushbutton_layout);
        }

        Command_ToolBox.append(command_toolbox_tmp);
    }
}

void ATModeEdit::CommandListToolboxClear()
{
    for(int i = 0; i < Command_ToolBox.count(); i++)
    {
        Command_ToolBox[i]->CommandList.clear();
    }
    Command_ToolBox.clear();
}


void ATModeEdit::on_toolbox_pushbutton_clicked()
{
    QPushButton* clicked_button = qobject_cast<QPushButton*>(sender());

    qDebug() << clicked_button->text();

    for(int i = 0; i < Command_ToolBox.count(); i++)
    {
        for(int j = 0; j < Command_ToolBox[i]->CommandList.count(); j++)
        {
            if(Command_ToolBox[i]->CommandList[j].CommandName == clicked_button->text())
            {
                QString commandname_tmp = Command_ToolBox[i]->CommandList[j].CommandName;
                ui->Command_lineEdit->setText(commandname_tmp.remove(0,3));
                ui->Categoty_LineEdit->setText(Command_ToolBox[i]->CategoryName);
                ui->Description_TextEdit->setText(Command_ToolBox[i]->CommandList[j].Discription);
            }
        }
    }
}

void ATModeEdit::on_Export_PushButton_clicked()
{
    QFileDialog *file_dialog = new QFileDialog(NULL);
    file_dialog->setAcceptMode(QFileDialog::AcceptSave);
    file_dialog->setWindowTitle("Choose a file name");
    file_dialog->setNameFilter("Configuration File(*.ini)");

    if(file_dialog->exec() == QDialog::Accepted)
    {
        QString file_path = file_dialog->selectedFiles().at(0);

        if(Settings.exportSettingFile(Setting_c::ATSettings,file_path) == true)
        {
            QMessageBox::information(this,
                                     "Export Success",
                                     "The Command List Successfully Export");
        } else {
            QMessageBox::information(this,
                                     "Export Failed",
                                     "Please Check the File Path");
        }
    }
    return;
}


void ATModeEdit::on_Import_PushButton_clicked()
{
    QFileDialog *file_dialog = new QFileDialog(NULL);
    file_dialog->setAcceptMode(QFileDialog::AcceptOpen);
    file_dialog->setWindowTitle("Choose a file name");
    file_dialog->setNameFilter("Configuration File(*.ini)");

    if(file_dialog->exec() == QDialog::Accepted)
    {
        QString file_path = file_dialog->selectedFiles().at(0);
        if(Settings.importSettingFile(Setting_c::ATSettings,file_path) == true)
        {
            QMessageBox::information(this,
                                     "Import Success",
                                     "The Command List Successfully Import\n"
                                     "Please Restart The Program");
        } else {
            QMessageBox::information(this,
                                     "Import Failed",
                                     "Please Check the File Path");
        }
    }
    return;
}

