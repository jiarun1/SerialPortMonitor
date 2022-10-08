#include "mySerialPort.h"
#include <QMessageBox>
#include <iostream>
#include <QTime>
#include <QDateTime>

mySerialPort::mySerialPort()
{

}

void mySerialPort::Init()
{
    port = new QSerialPort();
    send_data_length = 0;
}

void mySerialPort::ReceiveEnable()
{
    connect(port,SIGNAL(readyRead()),this,SLOT(ReceiveData()));
}


QList<QSerialPortInfo> mySerialPort::getAvailablePortsInfo()
{
    available_ports_info = QSerialPortInfo::availablePorts();
    return available_ports_info;
}

QStringList mySerialPort::getAvailablePortsName()
{
    QStringList available_ports_name;
    available_ports_info = QSerialPortInfo::availablePorts();

    available_ports_name.clear();

    for(int i = 0; i < available_ports_info.count(); i++)
    {
        available_ports_name.append(available_ports_info[i].portName());
    }
    return available_ports_name;
}

mySerialPort::Port_Status_e mySerialPort::openPort(QString port_name)
{
    if(port->isOpen())
    {
        port->clear();
        port->close();
    }

    port->setPortName(port_name);

    if(!port->open(QSerialPort::ReadWrite))
    {
        return PORT_OPEN_FAIL;
    }
    ReceiveEnable();
    return PORT_WORK_WELL;
}

mySerialPort::Port_Status_e mySerialPort::openPort(QString port_name,
                                                   qint32 baud_rate,
                                                   QSerialPort::DataBits data_bits,
                                                   QSerialPort::Parity parity,
                                                   QSerialPort::StopBits stop_bits,
                                                   QSerialPort::FlowControl flow)
{
    if(port->isOpen()) {
        port->clear();
        port->close();
    }

    port->setPortName(port_name);

    if(!port->open(QSerialPort::ReadWrite)) {
        return PORT_OPEN_FAIL;
    }
    if(!port->setBaudRate(baud_rate)) {
        return PORT_SETBAUDRATE_FAIL;
    }
    if(!port->setDataBits(data_bits)) {
        return PORT_SETDATABITS_FAIL;
    }
    if(!port->setParity(parity)) {
        return PORT_SETPARITY_FAIL;
    }
    if(!port->setStopBits(stop_bits)) {
        return PORT_SETSTOPBITS_FAIL;
    }
    if(!port->setFlowControl(flow)) {
        return PORT_SETFLOWCONTROL_FAIL;
    }

    ReceiveEnable();
    return PORT_WORK_WELL;
}

void mySerialPort::ReceiveData()
{
    if(!port->isOpen()){
        return;
    }

    QByteArray data_in_bytes = port->readAll();

    //XXX: remove send string
//    data_in_bytes.remove(0,send_data_length);
//    send_data_length = 0;


    QString data_in_qstring = QString(data_in_bytes);
//    QString data_in_qstring = QString::fromLocal8Bit(data_in_bytes.toStdString().c_str());
    QTime current_time = QTime::currentTime();
    QString format = "%1:%2:%3:%4";
    QString timestamp = format.arg(current_time.hour()).arg(current_time.minute()).arg(current_time.second()).arg(current_time.msec());

    QStringList StoreData;
    StoreData.clear();
    StoreData.insert(0,timestamp);
    StoreData.insert(1,data_in_qstring);

    ReceivedData.append(StoreData);

    emit NewDataReady();
}

QList<QStringList> mySerialPort::Receive()
{
    return ReceivedData;
}

qint64 mySerialPort::Write(QString output)
{
    QByteArray output_bytearray = output.toLatin1();
    send_data_length = port->write(output_bytearray);
    return send_data_length;
}

void mySerialPort::Close()
{
    port->close();
}







