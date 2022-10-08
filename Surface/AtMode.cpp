#include "AtMode.h"
#include "ui_AtMode.h"
#include "Setting.h"
#include "Middle/SendReceiveArea.h"

ATMode::ATMode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ATMode)
{
    ui->setupUi(this);

    this->setWindowTitle("AtMode");
}

ATMode::~ATMode()
{
    delete ui;
}

void ATMode::Init()
{
    RefreshToolbox();
}


void ATMode::on_History_ListWidget_currentTextChanged(const QString &currentText)
{
    for(int i = 0; i < Command_ToolBox.count(); i++)
    {
        for(int j = 0; j < Command_ToolBox[i]->CommandList.count(); j++)
        {
            if(Command_ToolBox[i]->CommandList[j].CommandName == currentText)
            {
                QString commandname_tmp = Command_ToolBox[i]->CommandList[j].CommandName;
                ui->CommandEdit_LineEdit->setText(commandname_tmp);
                ui->Discrbion_Label->setText(QString("Description: ").append(Command_ToolBox[i]->CommandList[j].Discription));
                return;
            }
        }
    }
    ui->CommandEdit_LineEdit->setText(currentText);
    ui->Discrbion_Label->setText(QString("Description: "));
}


void ATMode::on_Send_PushButton_clicked()
{
    SendReceiveArea.Send(ui->CommandEdit_LineEdit->text());
    ui->History_ListWidget->addItem(ui->CommandEdit_LineEdit->text());
}


void ATMode::on_Clear_PushButton_clicked()
{
    ui->CommandEdit_LineEdit->clear();
    ui->Discrbion_Label->setText("Description: ");
}


void ATMode::on_Edit_PushButton_clicked()
{
    ATCommandEditWindow.Init();
    ATCommandEditWindow.show();
}

void ATMode::RefreshToolbox()
{
    QToolBox* toolbox = ui->CommandList_ToolBox;

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
                pushbutton_layout->addWidget(command_pushbutton_tmp);
                connect(command_pushbutton_tmp,SIGNAL(clicked()),this,SLOT(on_toolbox_pushbutton_clicked()));
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

void ATMode::CommandListToolboxClear()
{
    for(int i = 0; i < Command_ToolBox.count(); i++)
    {
        Command_ToolBox[i]->CommandList.clear();
    }
    Command_ToolBox.clear();
}

void ATMode::on_toolbox_pushbutton_clicked()
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
                ui->CommandEdit_LineEdit->setText(commandname_tmp);
                ui->Discrbion_Label->setText(QString("Description: ").append(Command_ToolBox[i]->CommandList[j].Discription));
            }
        }
    }
}
