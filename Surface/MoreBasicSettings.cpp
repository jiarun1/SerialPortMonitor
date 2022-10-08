#include "MoreBasicSettings.h"
#include "ui_MoreBasicSettings.h"
#include "Middle/SerialPortBasicSetting.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QSplitter>
#include <QTimer>


MoreBasicSettings::MoreBasicSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MoreBasicSettings)
{
    ui->setupUi(this);

    ui->PortDetail_TreeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);



}

void MoreBasicSettings::Init()
{
    Timer = new QTimer(this);
    Timer->start(COUNTER_UPDATE_PERIOD);
    connect(Timer,SIGNAL(timeout()),this,SLOT(on_Port_Refresh_clicked()));


    QString SettingValue = SerialPortBasicSetting.getCurrentSelectBaudRate();
    ui->BaudRate_LineEdit->setText(SettingValue);

    SettingValue = SerialPortBasicSetting.getCurrentSelectPort();
    on_Port_Refresh_clicked();
    ui->Port_Combo->setCurrentText(SettingValue);

    SettingValue = SerialPortBasicSetting.getCurrentSelectDataBits();
    ui->DataBits_Combo->clear();
    ui->DataBits_Combo->addItem("5");
    ui->DataBits_Combo->addItem("6");
    ui->DataBits_Combo->addItem("7");
    ui->DataBits_Combo->addItem("8");
    ui->DataBits_Combo->setCurrentText(SettingValue);

    SettingValue = SerialPortBasicSetting.getCurrentSelectParity();
    ui->Parity_Combo->clear();
    ui->Parity_Combo->addItem("None");
    ui->Parity_Combo->addItem("Even");
    ui->Parity_Combo->addItem("Odd");
    ui->Parity_Combo->addItem("Mark");
    ui->Parity_Combo->addItem("Space");
    ui->Parity_Combo->setCurrentText(SettingValue);

    SettingValue = SerialPortBasicSetting.getCurrentSelectStopBits();
    ui->StopBits_Combo->clear();
    ui->StopBits_Combo->addItem("One");
    ui->StopBits_Combo->addItem("OneAndHalf");
    ui->StopBits_Combo->addItem("Two");
    ui->StopBits_Combo->setCurrentText(SettingValue);

    SettingValue = SerialPortBasicSetting.getCurrentSelectFlowControl();
    ui->FlowControl_Combo->clear();
    ui->FlowControl_Combo->addItem("None");
    ui->FlowControl_Combo->addItem("Hardware");
    ui->FlowControl_Combo->addItem("Software");
    ui->FlowControl_Combo->setCurrentText(SettingValue);
}

MoreBasicSettings::~MoreBasicSettings()
{
    delete ui;
}


void MoreBasicSettings::on_BaudRate_LineEdit_textChanged(const QString &arg1)
{
    SerialPortBasicSetting.CurrentSelectBaudRateChange(arg1);
}


void MoreBasicSettings::on_DataBits_Combo_currentTextChanged(const QString &arg1)
{
    SerialPortBasicSetting.CurrentSelectDataBitsChange(arg1);
}


void MoreBasicSettings::on_Port_Combo_currentTextChanged(const QString &arg1)
{
    SerialPortBasicSetting.CurrentSelectPortChange(arg1);
}


void MoreBasicSettings::on_Parity_Combo_currentTextChanged(const QString &arg1)
{
    SerialPortBasicSetting.CurrentSelectParityChange(arg1);
}


void MoreBasicSettings::on_FlowControl_Combo_currentTextChanged(const QString &arg1)
{
    SerialPortBasicSetting.CurrentSelectFlowControlChange(arg1);
}


void MoreBasicSettings::on_StopBits_Combo_currentTextChanged(const QString &arg1)
{
    SerialPortBasicSetting.CurrentSelectStopBitsChange(arg1);
}


void MoreBasicSettings::on_Cancel_PushButton_clicked()
{
    this->close();
}


void MoreBasicSettings::on_PortOpenClose_PushButton_clicked()
{
    emit MoreBasicSettingOpenPort();
    this->close();
}


void MoreBasicSettings::on_Port_Refresh_clicked()
{
    available_port_info = SerialPortBasicSetting.getAvailablePortsInfo();

    QString previous_select_item = ui->Port_Combo->currentText();

    QString previous_select_treewidget_name;
    QTreeWidgetItem *previous_select_treewidget_item = ui->PortDetail_TreeWidget->currentItem();
    if(previous_select_treewidget_item != NULL)
    {
        previous_select_treewidget_name = ui->PortDetail_TreeWidget->currentItem()->text(0);
    }

    ui->Port_Combo->clear();
    ui->PortDetail_TreeWidget->clear();
    for(int i = 0; i < available_port_info.count(); i++)
    {
        ui->Port_Combo->addItem(available_port_info[i].portName());
        QTreeWidgetItem* newItem;
        newItem = new QTreeWidgetItem;
        newItem->setData(0,0,available_port_info[i].portName());
        newItem->setData(1,0,available_port_info[i].description());
        newItem->setData(2,0,available_port_info[i].manufacturer());

        ui->PortDetail_TreeWidget->addTopLevelItem(newItem);
        if(available_port_info[i].portName() == previous_select_treewidget_name)
        {
            ui->PortDetail_TreeWidget->setCurrentItem(newItem);
        }
    }

    ui->Port_Combo->setCurrentText(previous_select_item);

}




void MoreBasicSettings::on_PortDetail_TreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    ui->Port_Combo->setCurrentText(item->text(0));
}

