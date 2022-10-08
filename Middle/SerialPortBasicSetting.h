#ifndef SERIALPORTTOOL_H
#define SERIALPORTTOOL_H

#include "Underlying/mySerialPort.h"
#include <QObject>
#include <QString>
#include <QList>
#include <QStringList>

class SerialPortBasicSetting_c : public QObject
{
    Q_OBJECT

public:
    void Init();
    void CurrentSelectPortChange(QString arg1);
    void CurrentSelectBaudRateChange(QString arg1);
    void CurrentSelectDataBitsChange(QString arg1);
    void CurrentSelectParityChange(QString arg1);
    void CurrentSelectStopBitsChange(QString arg1);
    void CurrentSelectFlowControlChange(QString arg1);

    QString getCurrentSelectPort();
    QString getCurrentSelectBaudRate();
    QString getCurrentSelectDataBits();
    QString getCurrentSelectParity();
    QString getCurrentSelectStopBits();
    QString getCurrentSelectFlowControl();



    QStringList getAvailablePortsName();
    QList<QSerialPortInfo> getAvailablePortsInfo();

    mySerialPort* getPortAddress();


    mySerialPort::Port_Status_e OpenPort();
    void ClosePort();



private:
    mySerialPort* SerialPort;

    QString CurrentSelectPort;
    QString CurrentSelectBaudRate;
    QString CurrentSelectDataBits;
    QString CurrentSelectParity;
    QString CurrentSelectStopBits;
    QString CurrentFlowControl;
};

extern SerialPortBasicSetting_c SerialPortBasicSetting;

#endif // SERIALPORTTOOL_H
