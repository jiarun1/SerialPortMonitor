#ifndef RECEIVEAREA_H
#define RECEIVEAREA_H

#include <QObject>
#include <QList>
#include <QString>
#include <QStringList>
#include <QTextBrowser>
#include <QTextEdit>
#include "Underlying/mySerialPort.h"
#include "Middle/SerialPortBasicSetting.h"

class SendReceiveArea_c : public QObject
{
    Q_OBJECT
public:
    SendReceiveArea_c();

    void Init();

    QList<QStringList> Receive();
    QStringList getLatestReceive();
    qint64 Write(QString output);
    qint64 Send(QString data_in_string);

    qint64 getSendDataCounter();
    qint64 getReceiveDataCounter();

    void setDisplayArea(QTextBrowser* text_browser);
    void DisplaySelectData(QStringList data,QTextBrowser* text_browser, qint64 time_interval = LONG_LONG_MAX, bool send_receive_state = false);
    void DisplayLogInTextBroser(QTextBrowser* text_browser);
    void DisplayLatestData(QTextBrowser* text_browser);


    void ReceiveAndSaveSetPath(QString path);

    void ReceiveAndSaveStateChanged(bool state);
    void SendMsgDisplayStateChanged(bool state);
    void ReceiveMsgDisplayStateChanged(bool state);
    void HexDisplayStateChanged(bool state);
    void AutoFrameBreakChanged(bool state);
    void AutoFrameBreakNumberChanged(int frame);
    void TimeStampDisplayStateChange(bool state);
    void HexSendStateChange(bool state);

    void AutoLineBreakChange(QString line_break);
    void AutoClearSendAreaChange(bool state);

    bool getReceiveAndSaveState();
    bool getSendMsgDisplatState();
    bool getReceiveMsgDisplaystate();
    bool getHexDisplayState();
    bool getTimeStampDisplayState();
    bool getAutoFrameBreakState();
    int getAutoFrameBreakFrame();
    bool getHexSendState();

    QString getAutoLineBreakString();
    bool getAutoClearSendAreaState();


    bool saveLogData(QString file_path);

    void clearData();
    void clearSendDataCounter();
    void clearReceiveDataCounter();

    void updateDisplayArea(QTextBrowser* text_browser);

private slots:
    void receiveNewDataSignal();

signals:
    void NewDataAvailable(QStringList Newdata);


private:



private:
    mySerialPort* SerialPort;
    QTextBrowser* DisplayArea;

    qint64 SendNumber;
    qint64 ReceiveNumber;

    bool ReceiveAndSave;
    QString ReceiveAndSavePath;


    bool TimeStampDisplay;
    bool SendMsgDisplay;
    bool ReceiveMsgDisplay;
    bool HexDisplay;
    bool AutoFrameBreakState;
    int  AutoFrameBreakFrameNumber;

    bool HexSend;


    QString AutoLineBreak;
    bool AutoClearSendArea;



    QStringList LatestData;
    qint64 LatestDataTimeInterval;


    QList<QStringList> CommunicateLog;


};

extern SendReceiveArea_c SendReceiveArea;

#endif // RECEIVEAREA_H
