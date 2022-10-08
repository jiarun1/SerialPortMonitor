#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "Middle/SerialPortBasicSetting.h"
#include "Middle/SendReceiveArea.h"
#include "Surface/MoreBasicSettings.h"
#include "Surface/AtMode.h"
#include "Surface/GraphWindow.h"

#ifndef PORT_REFRESH_PERIOD
#define PORT_REFRESH_PERIOD 1000//ms
#endif

extern SerialPortBasicSetting_c SerialPortBasicSetting;
extern SendReceiveArea_c SendReceiveArea;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void MoreBasicSetting_Active();

    void updateCounter();


private slots:
    void on_MoreSetting_PushButton_clicked();

    void on_PortOpenClose_PushButton_toggled(bool checked);

    void on_Port_Refresh_clicked();

    void on_Port_Combo_currentTextChanged(const QString &arg1);

    void on_BaudRate_Combo_currentTextChanged(const QString &arg1);

    void on_DataBits_Combo_currentTextChanged(const QString &arg1);

    void on_Parity_Combo_currentTextChanged(const QString &arg1);

    void on_StopBits_Combo_currentTextChanged(const QString &arg1);

    void on_SendData_PushButton_clicked();

    void on_SendData_TextEdit_textChanged();

    void on_ReceiveAndSave_CheckBox_stateChanged(int arg1);

    void on_HexDisplay_CheckBox_stateChanged(int arg1);

    void on_ReceiveDisplay_Checkbox_stateChanged(int arg1);

    void on_AutoFrameBreak_CheckBox_stateChanged(int arg1);

    void on_AutoFrameBreak_LineEdit_textChanged(const QString &arg1);

    void on_ReceiveScript_CheckBox_stateChanged(int arg1);

    void on_ReceiveScript_Combo_currentTextChanged(const QString &arg1);

    void on_SaveData_PushButton_clicked();

    void on_ClearData_PushButton_clicked();

    void on_SendFile_CheckBox_stateChanged(int arg1);

    void on_HexSend_CheckBox_stateChanged(int arg1);




    void on_SendScript_CheckBox_stateChanged(int arg1);

    void on_SendScript_Combo_currentTextChanged(const QString &arg1);

    void on_TimeDelivery_CheckBox_stateChanged(int arg1);

    void on_TimeDelivery_LineEdit_textChanged(const QString &arg1);

    void on_DTR_CheckBox_stateChanged(int arg1);

    void on_RTS_CheckBox_stateChanged(int arg1);

    void on_LineBreak_Combo_currentTextChanged(const QString &arg1);

    void on_ShowSendingString_CheckBox_stateChanged(int arg1);

    void on_AutoClearSendArea_Check_stateChanged(int arg1);

    void on_ShowTimeStamp_Check_stateChanged(int arg1);

    void on_action_ATMode_triggered();

    void on_action_GraphMode_triggered();

private:
    Ui::MainWindow *ui;

    GraphWindow Graph;

    QTimer *Timer;
    QTimer *CounterUpdateTimer;


    int pre_senddata_textedit_count;


    void BasicSettingAreaInit();
    void BasicSettingEnable(bool status);

    void SendReceiveAreaInit();


    QString getReceiveStringDataPath();


    MoreBasicSettings MoreBasicSettingsWindow;

    ATMode ATModeWindow;






};
#endif // MAINWINDOW_H
