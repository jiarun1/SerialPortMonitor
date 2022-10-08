#include "SendReceiveArea.h"
#include <QTime>
#include <QDebug>
#include "Setting.h"
#include <QFile>
#include "Underlying/qlibfile.h"
#include "MyFormatTrans.h"

SendReceiveArea_c SendReceiveArea;

using namespace MyFormatTrans;

SendReceiveArea_c::SendReceiveArea_c()
{

}

void SendReceiveArea_c::Init()
{
    SerialPort = SerialPortBasicSetting.getPortAddress();
    connect(SerialPort, SIGNAL(NewDataReady()), this, SLOT(receiveNewDataSignal()));
    DisplayArea = NULL;
    CommunicateLog.clear();
    LatestData.clear();

    ReceiveAndSave = Settings.getSendReceiveAreaValue("ReceiveAndSave",false).toBool();
    SendMsgDisplay = Settings.getSendReceiveAreaValue("SendMsgDisplay",true).toBool();
    ReceiveMsgDisplay = Settings.getSendReceiveAreaValue("ReceiveMsgDisplay",true).toBool();
    HexDisplay = Settings.getSendReceiveAreaValue("HexDisplay",false).toBool();
    AutoFrameBreakState = Settings.getSendReceiveAreaValue("AutoFrameBreakState",false).toBool();
    AutoFrameBreakFrameNumber = Settings.getSendReceiveAreaValue("AutoFrameBreakFrameNumber",1).toInt();
    TimeStampDisplay = Settings.getSendReceiveAreaValue("TimeStampDisplay",true).toBool();
    HexSend = Settings.getSendReceiveAreaValue("HexSend",false).toBool();

    AutoLineBreak = Settings.getSendReceiveAreaValue("AutoLineBreak","None").toString();
    AutoClearSendArea = Settings.getSendReceiveAreaValue("AutoClearSendArea",false).toBool();


    clearSendDataCounter();
    clearReceiveDataCounter();
}

void SendReceiveArea_c::setDisplayArea(QTextBrowser *text_browser)
{
    DisplayArea = text_browser;
}


void SendReceiveArea_c::DisplaySelectData(QStringList data, QTextBrowser *text_browser, qint64 time_interval, bool send_receive_state)
{
    QStringList new_data_list = data;


    if(HexDisplay == true)
    {
        QStringList data_in_hex;
        data_in_hex.clear();

        for(int i = 0; i < new_data_list.at(2).count(); i++)
        {
            QChar new_char = new_data_list[2].at(i);
            data_in_hex.append(QCharTOQStringHex(new_char));
        }
        new_data_list.removeLast();
        new_data_list.append(data_in_hex);
    }

    if(AutoFrameBreakState == true)
    {
        if(TimeStampDisplay == false)
        {
            new_data_list.removeFirst();
        }


        if((time_interval <= AutoFrameBreakFrameNumber) && (send_receive_state == true))
        {
            QString newdata = new_data_list.at(2);
            text_browser->insertPlainText(newdata);
        } else {
            QString newdata = new_data_list.join("  ");
            text_browser->append(newdata.append("\n"));
        }


    } else {
        QString newdata = new_data_list.at(2);
        text_browser->insertPlainText(newdata);
    }
}

void SendReceiveArea_c::DisplayLatestData(QTextBrowser *text_browser)
{
    qint64 time_interval = LONG_LONG_MAX;
    bool send_receive_same_state = false;
    if(CommunicateLog.count() >= 2)
    {
        time_interval = TimeIntervalInMicroSecond(CommunicateLog.at(CommunicateLog.count()-2).at(0),
                                                             CommunicateLog.last().at(0));

        if(CommunicateLog.at(CommunicateLog.count()-2).at(1) == CommunicateLog.last().at(1)) {
            send_receive_same_state = true;
        } else {
            send_receive_same_state = false;
        }
    }

    DisplaySelectData(LatestData,text_browser, time_interval, send_receive_same_state);
}

void SendReceiveArea_c::DisplayLogInTextBroser(QTextBrowser *text_browser)
{
    text_browser->clear();
    for(int i = 0; i < CommunicateLog.count(); i++)
    {
        DisplaySelectData(CommunicateLog[i],text_browser);
    }
}





void SendReceiveArea_c::ReceiveAndSaveStateChanged(bool state)
{
    ReceiveAndSave = state;
    Settings.setSendReceiveAreaValue("ReceiveAndSave",state);
}


void SendReceiveArea_c::ReceiveAndSaveSetPath(QString path)
{
    ReceiveAndSavePath = path;
}


void SendReceiveArea_c::SendMsgDisplayStateChanged(bool state)
{
    SendMsgDisplay = state;
    Settings.setSendReceiveAreaValue("SendMsgDisplay",state);
}

void SendReceiveArea_c::ReceiveMsgDisplayStateChanged(bool state)
{
    ReceiveMsgDisplay = state;
    Settings.setSendReceiveAreaValue("ReceiveMsgDisplay",state);
}

void SendReceiveArea_c::HexDisplayStateChanged(bool state)
{
    HexDisplay = state;
    Settings.setSendReceiveAreaValue("HexDisplay",state);
}

void SendReceiveArea_c::TimeStampDisplayStateChange(bool state)
{
    TimeStampDisplay = state;
    Settings.setSendReceiveAreaValue("TimeStampDisplay",state);
}

void SendReceiveArea_c::AutoFrameBreakChanged(bool state)
{
    AutoFrameBreakState = state;
    Settings.setSendReceiveAreaValue("AutoFrameBreakState",state);
}

void SendReceiveArea_c::AutoFrameBreakNumberChanged(int frame)
{
    AutoFrameBreakFrameNumber = frame;
    Settings.setSendReceiveAreaValue("AutoFrameBreakFrameNumber",frame);
}

void SendReceiveArea_c::HexSendStateChange(bool state)
{
    HexSend = state;
    Settings.setSendReceiveAreaValue("HexSend",state);
}


void SendReceiveArea_c::AutoLineBreakChange(QString line_break)
{
    AutoLineBreak = line_break;
    Settings.setSendReceiveAreaValue("AutoLineBreak",line_break);
}

void SendReceiveArea_c::AutoClearSendAreaChange(bool state)
{
    AutoClearSendArea = state;
    Settings.setSendReceiveAreaValue("AutoClearSendArea",state);
}




bool SendReceiveArea_c::getReceiveAndSaveState()
{
    return ReceiveAndSave;
}


bool SendReceiveArea_c::getSendMsgDisplatState()
{
    return SendMsgDisplay;
}

bool SendReceiveArea_c::getReceiveMsgDisplaystate()
{
    return ReceiveMsgDisplay;
}

bool SendReceiveArea_c::getHexDisplayState()
{
    return HexDisplay;
}

bool SendReceiveArea_c::getTimeStampDisplayState()
{
    return TimeStampDisplay;
}

int SendReceiveArea_c::getAutoFrameBreakFrame()
{
    return AutoFrameBreakFrameNumber;
}

bool SendReceiveArea_c::getAutoFrameBreakState()
{
    return AutoFrameBreakState;
}

bool SendReceiveArea_c::getHexSendState()
{
    return HexSend;
}

QString SendReceiveArea_c::getAutoLineBreakString()
{
    return AutoLineBreak;
}

bool SendReceiveArea_c::getAutoClearSendAreaState()
{
    return AutoClearSendArea;
}




QList<QStringList> SendReceiveArea_c::Receive()
{
    return SerialPort->Receive();
}

QStringList SendReceiveArea_c::getLatestReceive()
{
    QList<QStringList> Data = SerialPort->Receive();
    return Data.at(Data.count()-1);
}

qint64 SendReceiveArea_c::Write(QString output)
{
    QTime current_time = QTime::currentTime();
    QString format = "%1:%2:%3:%4";
    QString timestamp = format.arg(current_time.hour()).arg(current_time.minute()).arg(current_time.second()).arg(current_time.msec());

    LatestData.clear();
    LatestData.insert(0,timestamp);
    LatestData.insert(1,QString(">>"));
    LatestData.insert(2,output);

    CommunicateLog.append(LatestData);

    if((SendMsgDisplay == true)&&(DisplayArea != NULL))
    {
        DisplayLatestData(DisplayArea);
    }

    SendNumber += output.count();

    return SerialPort->Write(output);

}

qint64 SendReceiveArea_c::Send(QString data_in_string)
{
    QString output_string;
    output_string.clear();
    if(HexSend)
    {
        QStringList data = data_in_string.split(" ", Qt::SkipEmptyParts);
        for(int i = 0; i < data.count(); i++)
        {
            output_string.append(QStringTOQCharHex(data.at(i)));
        }
    } else {
        output_string = data_in_string;
    }

    if(AutoLineBreak == "None"){

    } else if (AutoLineBreak == "\\r\\n"){
        output_string.append("\r\n");
    } else if (AutoLineBreak == "\\n"){
        output_string.append("\n");
    }

    return Write(output_string);
}

void SendReceiveArea_c::receiveNewDataSignal()
{
    LatestData.clear();
    LatestData = getLatestReceive();
    LatestData.insert(1,"<<");
    CommunicateLog.append(LatestData);

    ReceiveNumber += LatestData.at(2).count();

    if((ReceiveMsgDisplay == true)&&(DisplayArea != NULL))
    {
        DisplayLatestData(DisplayArea);
    }

    emit NewDataAvailable(LatestData);
}

void SendReceiveArea_c::clearData()
{
    DisplayArea->clear();
    LatestData.clear();
    CommunicateLog.clear();
}

void SendReceiveArea_c::clearSendDataCounter()
{
    SendNumber = 0;
}

void SendReceiveArea_c::clearReceiveDataCounter()
{
    ReceiveNumber = 0;
}

qint64 SendReceiveArea_c::getSendDataCounter()
{
    return SendNumber;
}

qint64 SendReceiveArea_c::getReceiveDataCounter()
{
    return ReceiveNumber;
}


bool SendReceiveArea_c::saveLogData(QString file_path)
{
    FileLib::CheckPath(file_path);

    QFile file_name(file_path);

//    if(!file_name.exists())
//    {
//        return false;
//    }

    if(!file_name.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return false;
    }

    for(int i = 0; i < CommunicateLog.count(); i++)
    {
        QString one_line_data = CommunicateLog[i].join(",");
        one_line_data.append("\n");
        file_name.write(one_line_data.toLatin1());
    }
    file_name.close();
    return true;
}


void SendReceiveArea_c::updateDisplayArea(QTextBrowser* text_browser)
{

}


