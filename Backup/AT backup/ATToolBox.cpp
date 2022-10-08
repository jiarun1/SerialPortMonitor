#include "ATToolBox.h"
#include <QDebug>
#include "Setting.h"

ATToolBox At_ToolBox_Control;

ATToolBox::ATToolBox()
{

}

void ATToolBox::Init()
{

}

void ATToolBox::CommandListToolboxClear()
{
    for(int i = 0; i < Command_ToolBox.count(); i++)
    {
        Command_ToolBox[i]->CommandList.clear();
    }
    Command_ToolBox.clear();
}

void ATToolBox::RefreshCommandList(QToolBox* toolbox)
{
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


/*
 * @brief       :   used to add a new command
 *
 * @return      :
 *   @retval    :   -1  :   command exist
 */
int ATToolBox::addATCommand(const QString &command,const QString &category,const QString &discription, QToolBox* toolbox)
{
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
        return TOOLBOX_COMMAND_EXIST;
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
            }
        }

        command_pushbutton_tmp->setText(command);

        pushbutton_boxlyout_tmp->addWidget(command_pushbutton_tmp);

        category_widget_tmp->setLayout(pushbutton_boxlyout_tmp);
    }

    return TOOLBOX_WORKS_WELL;
}


/*
 * @brief       :   used to delete a new command
 *
 * @return      :
 *   @retval    :   -2  :   category is not exist
 *   @retval    :   -1  :   command is not exist
 */
int ATToolBox::deleteATCommand(const QString &command,const QString &category, QToolBox* toolbox)
{
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
        return TOOLBOX_CATEGORY_NOTEXIST;
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
        return TOOLBOX_COMMAND_NOTEXIST;
    }

    Settings.deleteATCommand(command,category);


    if((toolbox != nullptr) && (category_widget_tmp != nullptr))
    {
        delete category_widget_tmp->layout();
        QVBoxLayout* pushbutton_boxlyout_tmp = new QVBoxLayout;

        QObjectList exist_pushbutton_list = category_widget_tmp->children();
        for(int i = 0; i < exist_pushbutton_list.count();i++)
        {
            if(exist_pushbutton_list[i]->metaObject()->className() == QString("QPushButton"))
            {
                QPushButton* exist_pushbutton_tmp = qobject_cast<QPushButton*>(exist_pushbutton_list[i]);

                QString debug = exist_pushbutton_tmp->text();

                if(exist_pushbutton_tmp->text().toStdString() == command.toStdString())
                {
                    disconnect(exist_pushbutton_tmp,SIGNAL(clicked()),toolbox->parentWidget(),SLOT(on_toolbox_pushbutton_clicked));
                    exist_pushbutton_tmp->deleteLater();
                    continue;
                }
                pushbutton_boxlyout_tmp->addWidget(exist_pushbutton_tmp);
            }
        }

        category_widget_tmp->setLayout(pushbutton_boxlyout_tmp);
    }


    return TOOLBOX_WORKS_WELL;
}
