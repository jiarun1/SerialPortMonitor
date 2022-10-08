#include "Surface/mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDebug>
#include <QTextBrowser>
#include <QTextCursor>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include "Setting.h"
#include "MyFormatTrans.h"

using namespace MyFormatTrans;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/Icons/Port.png"));

    this->setWindowTitle("Serial Port Monitor");

    SerialPortBasicSetting.Init();
    SendReceiveArea.Init();
    BasicSettingAreaInit();
    SendReceiveAreaInit();

    SendReceiveArea.setDisplayArea(ui->DataDisplay_TextBrowser);

    pre_senddata_textedit_count = 0;
}

MainWindow::~MainWindow()
{
    MoreBasicSettingsWindow.close();
    delete ui;
}


/*
 * @brief       : init the basic setting area
 *                1. Refresh timer + first refresh
 *                2. update open button state
 *                3. BaudRate init
 *                4. DataBits init
 *                5. parity init
 *                6. stopbit init
 * @param[in]   : None
 * @return      : None
 * @usage       : MainWinsdow
 */
void MainWindow::BasicSettingAreaInit()
{
    Timer = new QTimer(this);
    connect(Timer,SIGNAL(timeout()),this,SLOT(on_Port_Refresh_clicked()));
    on_Port_Refresh_clicked();

    on_PortOpenClose_PushButton_toggled(ui->PortOpenClose_PushButton->isChecked());

    QString SettingValue = Settings.getBasicSettingValue("BaudRate","1200").toString();
    ui->BaudRate_Combo->clear();
    ui->BaudRate_Combo->addItem("1200");
    ui->BaudRate_Combo->addItem("2400");
    ui->BaudRate_Combo->addItem("4800");
    ui->BaudRate_Combo->addItem("9600");
    ui->BaudRate_Combo->addItem("19200");
    ui->BaudRate_Combo->addItem("38400");
    ui->BaudRate_Combo->addItem("57600");
    ui->BaudRate_Combo->addItem("115200");
    ui->BaudRate_Combo->setCurrentText(SettingValue);

    SettingValue = Settings.getBasicSettingValue("DataBits","5").toString();
    ui->DataBits_Combo->clear();
    ui->DataBits_Combo->addItem("5");
    ui->DataBits_Combo->addItem("6");
    ui->DataBits_Combo->addItem("7");
    ui->DataBits_Combo->addItem("8");
    ui->DataBits_Combo->setCurrentText(SettingValue);

    SettingValue = Settings.getBasicSettingValue("Parity","None").toString();
    ui->Parity_Combo->clear();
    ui->Parity_Combo->addItem("None");
    ui->Parity_Combo->addItem("Even");
    ui->Parity_Combo->addItem("Odd");
    ui->Parity_Combo->addItem("Mark");
    ui->Parity_Combo->addItem("Space");
    ui->Parity_Combo->setCurrentText(SettingValue);

    SettingValue = Settings.getBasicSettingValue("StopBits","One").toString();
    ui->StopBits_Combo->clear();
    ui->StopBits_Combo->addItem("One");
    ui->StopBits_Combo->addItem("OneAndHalf");
    ui->StopBits_Combo->addItem("Two");
    ui->StopBits_Combo->setCurrentText(SettingValue);
}


void MainWindow::MoreBasicSetting_Active()
{
    ui->PortOpenClose_PushButton->click();

    disconnect(&MoreBasicSettingsWindow,SIGNAL(MoreBasicSettingOpenPort()),this,SLOT(MoreBasicSetting_Active()));

    if(-1 == ui->BaudRate_Combo->findText(SerialPortBasicSetting.getCurrentSelectBaudRate()))
    {
        ui->BaudRate_Combo->addItem(SerialPortBasicSetting.getCurrentSelectBaudRate());
    }

    ui->BaudRate_Combo->setCurrentText(SerialPortBasicSetting.getCurrentSelectBaudRate());
    ui->Port_Combo->setCurrentText(SerialPortBasicSetting.getCurrentSelectPort());
    ui->DataBits_Combo->setCurrentText(SerialPortBasicSetting.getCurrentSelectDataBits());
    ui->Parity_Combo->setCurrentText(SerialPortBasicSetting.getCurrentSelectParity());
    ui->StopBits_Combo->setCurrentText(SerialPortBasicSetting.getCurrentSelectStopBits());


}

/*
 * @brief       : init the send and receive area
 *                1. init receive area check button
 * @param[in]   : None
 * @return      : None
 * @usage       : MainWinsdow
 */
void MainWindow::SendReceiveAreaInit()
{
    CounterUpdateTimer = new QTimer;
    connect(CounterUpdateTimer,SIGNAL(timeout()),this,SLOT(updateCounter()));

    CounterUpdateTimer->start(COUNTER_UPDATE_PERIOD);


//    ui->ReceiveAndSave_CheckBox

    ui->HexDisplay_CheckBox->setCheckState((SendReceiveArea.getHexDisplayState()?(Qt::Checked):Qt::Unchecked));

    ui->ReceiveDisplay_Checkbox->setCheckState((SendReceiveArea.getReceiveMsgDisplaystate()?(Qt::Checked):Qt::Unchecked));

    ui->AutoFrameBreak_CheckBox->setCheckState((SendReceiveArea.getAutoFrameBreakState()?(Qt::Checked):Qt::Unchecked));
    ui->AutoFrameBreak_LineEdit->setText(QString("%1").arg(SendReceiveArea.getAutoFrameBreakFrame()));

//    ui->ReceiveScript_CheckBox

//    ui->SendFile_CheckBox

    ui->HexSend_CheckBox->setCheckState((SendReceiveArea.getHexSendState()?(Qt::Checked):Qt::Unchecked));


    QString Settings = SendReceiveArea.getAutoLineBreakString();
    ui->LineBreak_Combo->clear();
    ui->LineBreak_Combo->addItem("None");
    ui->LineBreak_Combo->addItem("\\r\\n");
    ui->LineBreak_Combo->addItem("\\n");
    ui->LineBreak_Combo->setCurrentText(Settings);

    ui->AutoClearSendArea_Check->setCheckState(SendReceiveArea.getAutoClearSendAreaState()?(Qt::Checked):(Qt::Unchecked));


    ui->ShowTimeStamp_Check->setCheckState(SendReceiveArea.getTimeStampDisplayState()?(Qt::Checked):(Qt::Unchecked));
}




/*
 * @Brief       : click the item to open a new widget to enable extra settings
 * @param[in]   : None
 * @return      : None
 * @usage       : Slot for moresetting_pushbutton click signal
 */
void MainWindow::on_MoreSetting_PushButton_clicked()
{
    MoreBasicSettingsWindow.show();
    MoreBasicSettingsWindow.Init();
    connect(&MoreBasicSettingsWindow,SIGNAL(MoreBasicSettingOpenPort()),this,SLOT(MoreBasicSetting_Active()));
}


/*
 * @Brief       : click the button to open or close the port
 * @param[in]   : checked :  the button state after click the button
 * @return      : None
 * @usage       : Slot for PortOpenClose_PushButton click signal
 */
void MainWindow::on_PortOpenClose_PushButton_toggled(bool checked)
{
    if(checked == false){
        ui->PortOpenClose_PushButton->setText("Open Port");
        ui->PortOpenClose_PushButton->setChecked(false);
        BasicSettingEnable(true);
        Timer->start(PORT_REFRESH_PERIOD);
        SerialPortBasicSetting.ClosePort();
    } else {
        ui->PortOpenClose_PushButton->setText("Close Port");
        ui->PortOpenClose_PushButton->setChecked(true);
        BasicSettingEnable(false);
        Timer->stop();
        SerialPortBasicSetting.OpenPort();
    }

    SendReceiveArea.clearReceiveDataCounter();
    SendReceiveArea.clearSendDataCounter();
}

/*
 * @Brief       : set the basic setting is enable or disable
 * @param[in]   : status :
 *                        true  : enable
 *                        false : disable
 * @return      : None
 * @usage       : on_PortOpenClose_PushButton_toggled(bool checked)
 */
void MainWindow::BasicSettingEnable(bool status)
{
    ui->MoreSetting_PushButton->setEnabled(status);
    ui->Port_Combo->setEnabled(status);
    ui->BaudRate_Combo->setEnabled(status);
    ui->Parity_Combo->setEnabled(status);
    ui->DataBits_Combo->setEnabled(status);
    ui->StopBits_Combo->setEnabled(status);
    ui->Port_Refresh->setEnabled(status);
}


void MainWindow::on_Port_Refresh_clicked()
{
    QString previous_select_item = ui->Port_Combo->currentText();
    QStringList port_name_list = SerialPortBasicSetting.getAvailablePortsName();
    ui->Port_Combo->clear();
    for(int i = 0; i < port_name_list.count(); i++)
    {
        ui->Port_Combo->addItem(port_name_list[i]);
    }
    ui->Port_Combo->setCurrentText(previous_select_item);
}



/*
 * @Brief       : deal with the Port current index changed signal
 * @param[in]   : arg1 : New Port Name
 * @return      : None
 * @usage       : Slot : Port_Combo_currentIndexChanged
 */
void MainWindow::on_Port_Combo_currentTextChanged(const QString &arg1)
{
    SerialPortBasicSetting.CurrentSelectPortChange(arg1);
}

/*
 * @Brief       : deal with the BaudRate current index changed signal
 * @param[in]   : arg1 : New Baundrate value
 * @return      : None
 * @usage       : Slot : BaudRate_Combo_currentIndexChanged
 */
void MainWindow::on_BaudRate_Combo_currentTextChanged(const QString &arg1)
{
    SerialPortBasicSetting.CurrentSelectBaudRateChange(arg1);
}

/*
 * @Brief       : deal with the DataBits current index changed signal
 * @param[in]   : arg1 : New DataBits value
 * @return      : None
 * @usage       : Slot : DataBits_Combo_currentIndexChanged
 */
void MainWindow::on_DataBits_Combo_currentTextChanged(const QString &arg1)
{
    SerialPortBasicSetting.CurrentSelectDataBitsChange(arg1);
}

/*
 * @Brief       : deal with the Parity current index changed signal
 * @param[in]   : arg1 : New Parity Value
 * @return      : None
 * @usage       : Slot : Parity_Combo_currentIndexChanged
 */
void MainWindow::on_Parity_Combo_currentTextChanged(const QString &arg1)
{
     SerialPortBasicSetting.CurrentSelectParityChange(arg1);
}

/*
 * @Brief       : deal with the StopBits current index changed signal
 * @param[in]   : arg1 : New StopBits Value
 * @return      : None
 * @usage       : Slot : StopBits_Combo_currentIndexChanged
 */
void MainWindow::on_StopBits_Combo_currentTextChanged(const QString &arg1)
{
     SerialPortBasicSetting.CurrentSelectStopBitsChange(arg1);
}


void MainWindow::on_SendData_PushButton_clicked()
{
    SendReceiveArea.Send(ui->SendData_TextEdit->toPlainText());
    if(ui->AutoClearSendArea_Check->checkState() == Qt::Checked)
    {
        ui->SendData_TextEdit->clear();
    }
}


void MainWindow::on_SendData_TextEdit_textChanged()
{
    QString text_edit_data = ui->SendData_TextEdit->toPlainText();
    int text_edit_data_length = text_edit_data.count();

    if(text_edit_data_length < 1){
        return;
    }

    QChar last_qchar = text_edit_data.at(text_edit_data_length-1);

    if(pre_senddata_textedit_count > text_edit_data_length)
    {
        pre_senddata_textedit_count = text_edit_data_length;
        if(last_qchar == ' ')
        {
            text_edit_data.chop(1);
//            pre_senddata_textedit_count--;
            goto NewTextSet;
        }
        return;
    }
    pre_senddata_textedit_count = text_edit_data_length;


    if(ui->HexSend_CheckBox->checkState() == Qt::Checked)
    {
        if((last_qchar >= '0') && (last_qchar <= '9'))
        {

        } else if ((last_qchar >= 'a') && (last_qchar <= 'f')) {
            last_qchar = last_qchar.toUpper();
            text_edit_data.chop(1);
            text_edit_data.append(last_qchar);
            ui->SendData_TextEdit->setText(text_edit_data);
            QTextCursor end_cursor = ui->SendData_TextEdit->textCursor();
            end_cursor.movePosition(QTextCursor::End);
            ui->SendData_TextEdit->setTextCursor(end_cursor);
        } else if ((last_qchar >= 'A') && (last_qchar <= 'F')){

        } else if (last_qchar == ' ' ){

        } else {
            text_edit_data.chop(1);
            goto NewTextSet;
        }

        if((text_edit_data_length % 3) == 2)
        {
            text_edit_data.append(" ");
            goto NewTextSet;
        } else if (((text_edit_data_length % 3) == 0)&&(last_qchar != ' ')) {
            text_edit_data.insert(text_edit_data_length - 1,' ');
            goto NewTextSet;
        } else{
            return;
        }
    }
    else {
        return;
    }

    NewTextSet:
    ui->SendData_TextEdit->setText(text_edit_data);
    QTextCursor end_cursor = ui->SendData_TextEdit->textCursor();
    end_cursor.movePosition(QTextCursor::End);
    ui->SendData_TextEdit->setTextCursor(end_cursor);
    return;

}


void MainWindow::on_ReceiveAndSave_CheckBox_stateChanged(int arg1)
{
    SendReceiveArea.ReceiveAndSaveStateChanged(arg1);

    if(arg1 == Qt::Checked)
    {
        QString file_path = getReceiveStringDataPath();

        if(file_path == "")
        {
            ui->ReceiveAndSave_CheckBox->setCheckState(Qt::Unchecked);
            return;
        }


    }
}


void MainWindow::on_HexDisplay_CheckBox_stateChanged(int arg1)
{
    SendReceiveArea.HexDisplayStateChanged(arg1);
}



void MainWindow::on_ReceiveDisplay_Checkbox_stateChanged(int arg1)
{
    SendReceiveArea.ReceiveMsgDisplayStateChanged(arg1);
}


void MainWindow::on_AutoFrameBreak_CheckBox_stateChanged(int arg1)
{
    SendReceiveArea.AutoFrameBreakChanged(arg1);
}


void MainWindow::on_AutoFrameBreak_LineEdit_textChanged(const QString &arg1)
{
    SendReceiveArea.AutoFrameBreakNumberChanged(arg1.toInt());
}


void MainWindow::on_ReceiveScript_CheckBox_stateChanged(int arg1)
{

}

void MainWindow::on_ReceiveScript_Combo_currentTextChanged(const QString &arg1)
{

}




void MainWindow::on_SaveData_PushButton_clicked()
{

    QString file_path = getReceiveStringDataPath();

    if(file_path == "")
    {
        return;
    }


    if(!SendReceiveArea.saveLogData(file_path))
    {
        QMessageBox::information(this,
                                 "Save File Failed",
                                 "Please Check The Selected Path");
    }

}


void MainWindow::on_ClearData_PushButton_clicked()
{
    SendReceiveArea.clearData();
}


void MainWindow::on_SendFile_CheckBox_stateChanged(int arg1)
{
    QString file_name = QFileDialog::getOpenFileName(this,
                                                     tr("Choose sending file"),
                                                     tr(""),
                                                     tr("Text(*.txt);;All Files(*.*)"));
    //TODO: Add Send File Code
}


void MainWindow::on_HexSend_CheckBox_stateChanged(int arg1)
{
    QString text_edit_data = ui->SendData_TextEdit->toPlainText();
    QString new_data;
    new_data.clear();
    if(arg1 == Qt::Checked)
    {
        for(int i = 0; i < text_edit_data.count(); i++)
        {
//            if((i % 3) == 1)
//            {
//                new_data.append(" ");
//                continue;
//            }

            QString tmp_string = QCharTOQStringHex(text_edit_data[i]);
            tmp_string.append(" ");
            new_data.append(tmp_string);
        }
    } else {
       QStringList text_edit_data_split = text_edit_data.split(" ",Qt::SkipEmptyParts);

       for(int i = 0; i < text_edit_data_split.count(); i++)
       {
           new_data.append(QStringTOQCharHex(text_edit_data_split.at(i)));
       }
    }

    ui->SendData_TextEdit->setText(new_data);

    SendReceiveArea.HexSendStateChange(arg1);
}

QString MainWindow::getReceiveStringDataPath()
{
    QFileDialog *file_dialog = new QFileDialog(NULL);
    file_dialog->setAcceptMode(QFileDialog::AcceptSave);
    file_dialog->setWindowTitle("Choose a file name");
    file_dialog->setNameFilter("text files(*.txt);;csv files(*.csv);;all files(*.*)");

    if(file_dialog->exec() == QDialog::Accepted)
    {
        QString file_path = file_dialog->selectedFiles().at(0);
        QFileInfo file_info(file_path);
        QString select_name_filter = file_dialog->selectedNameFilter();
        QString file_suffix = file_info.suffix().toLower();
        if((file_suffix != "txt")&&(file_suffix != "csv"))
        {
            if(select_name_filter == "text files(*.txt)")
            {
                file_path += ".txt";
            } else if (select_name_filter == "csv files(*.csv)") {
                file_path += ".csv";
            }
        }
        return file_path;
    }
    return "";
}




void MainWindow::on_SendScript_CheckBox_stateChanged(int arg1)
{

}


void MainWindow::on_SendScript_Combo_currentTextChanged(const QString &arg1)
{

}


void MainWindow::on_TimeDelivery_CheckBox_stateChanged(int arg1)
{

}


void MainWindow::on_TimeDelivery_LineEdit_textChanged(const QString &arg1)
{

}


void MainWindow::on_DTR_CheckBox_stateChanged(int arg1)
{

}


void MainWindow::on_RTS_CheckBox_stateChanged(int arg1)
{

}



void MainWindow::on_LineBreak_Combo_currentTextChanged(const QString &arg1)
{
    SendReceiveArea.AutoLineBreakChange(arg1);
}


void MainWindow::on_ShowSendingString_CheckBox_stateChanged(int arg1)
{
    SendReceiveArea.SendMsgDisplayStateChanged(arg1);
}


void MainWindow::on_AutoClearSendArea_Check_stateChanged(int arg1)
{

}


void MainWindow::on_ShowTimeStamp_Check_stateChanged(int arg1)
{
    SendReceiveArea.TimeStampDisplayStateChange(arg1);
}


void MainWindow::updateCounter()
{
    ui->SendNum_Label->setText(QString("Send: %1").arg(SendReceiveArea.getSendDataCounter()));
    ui->ReceiveNum_Label->setText(QString("Receive: %1").arg(SendReceiveArea.getReceiveDataCounter()));
}


void MainWindow::on_action_ATMode_triggered()
{
    ui->HexSend_CheckBox->setCheckState(Qt::Unchecked);
    ATModeWindow.Init();
    ATModeWindow.show();
}


void MainWindow::on_action_GraphMode_triggered()
{

    Graph.show();
}

