#ifndef ATTOOLBOX_H
#define ATTOOLBOX_H

#include <QWidget>
#include <QString>
#include <QList>
#include <QToolBox>
#include <QPushButton>
#include <QStringList>
#include <QVBoxLayout>

class ATToolBox : public QObject
{
public:
    ATToolBox();

    void Init();

    void RefreshCommandList(QToolBox* toolbox = nullptr);

    void CommandListToolboxClear();

    int addATCommand(const QString &command,const QString &category,const QString &discription, QToolBox* toolbox = nullptr);

    int deleteATCommand(const QString &command,const QString &category, QToolBox* toolbox = nullptr);

signals:

private slots:
    void PushButtonClicked();

private:

    enum{
        TOOLBOX_WORKS_WELL          = 0,
        TOOLBOX_COMMAND_EXIST       = -1,
        TOOLBOX_COMMAND_NOTEXIST    = -2,
        TOOLBOX_CATEGORY_NOTEXIST   = -3,

    };


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
};

extern ATToolBox At_ToolBox_Control;

#endif // ATTOOLBOX_H
