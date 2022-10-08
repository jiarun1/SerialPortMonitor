#ifndef MYSERIALPORT_H
#define MYSERIALPORT_H

#include <QObject>
#include <QList>
#include <QString>
#include <QStringList>
#include <QIODevice>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class mySerialPort : public QObject
{
    Q_OBJECT

public://enum
    typedef enum{
        PORT_WORK_WELL = 0,
        PORT_OPEN_FAIL,
        PORT_SETBAUDRATE_FAIL,
        PORT_SETDATABITS_FAIL,
        PORT_SETPARITY_FAIL,
        PORT_SETSTOPBITS_FAIL,
        PORT_SETFLOWCONTROL_FAIL,

    }Port_Status_e;

public://function
    mySerialPort();

    void ReceiveEnable();
    void Close();
    void Init();
    QList<QSerialPortInfo> getAvailablePortsInfo(void);
    QStringList getAvailablePortsName(void);

    Port_Status_e openPort(QString port_name);
    Port_Status_e openPort(QString port_name, qint32 baud_rate, QSerialPort::DataBits data_bits, QSerialPort::Parity parity, QSerialPort::StopBits stop_bits, QSerialPort::FlowControl flow);

    QList<QStringList> Receive();
    qint64 Write(QString output);


private:
    QList<QSerialPortInfo> available_ports_info;

    QList<QStringList> ReceivedData;

    QSerialPort *port;

    qint64 send_data_length;

signals:
    void NewDataReady();

private slots:
    void ReceiveData();


};

#endif // MYSERIALPORT_H
