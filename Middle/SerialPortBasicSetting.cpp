#include "Middle/SerialPortBasicSetting.h"
#include "Setting.h"

SerialPortBasicSetting_c SerialPortBasicSetting;

mySerialPort* SerialPortBasicSetting_c::getPortAddress()
{
    return SerialPort;
}

void SerialPortBasicSetting_c::Init()
{
    SerialPort = new mySerialPort;
    SerialPort->Init();
}

void SerialPortBasicSetting_c::CurrentSelectPortChange(QString arg1)
{
    CurrentSelectPort = arg1;
    Settings.setBasicSettingValue("Port",arg1);
}

void SerialPortBasicSetting_c::CurrentSelectBaudRateChange(QString arg1)
{
    CurrentSelectBaudRate = arg1;
    Settings.setBasicSettingValue("BaudRate",arg1);
}

void SerialPortBasicSetting_c::CurrentSelectDataBitsChange(QString arg1)
{
    CurrentSelectDataBits = arg1;
    Settings.setBasicSettingValue("DataBits",arg1);
}

void SerialPortBasicSetting_c::CurrentSelectParityChange(QString arg1)
{
    CurrentSelectParity = arg1;
    Settings.setBasicSettingValue("Parity",arg1);
}

void SerialPortBasicSetting_c::CurrentSelectStopBitsChange(QString arg1)
{
    CurrentSelectStopBits = arg1;
    Settings.setBasicSettingValue("StopBits",arg1);
}

void SerialPortBasicSetting_c::CurrentSelectFlowControlChange(QString arg1)
{
    CurrentFlowControl = arg1;
    Settings.setBasicSettingValue("FlowControl",arg1);
}

QString SerialPortBasicSetting_c::getCurrentSelectPort()
{
    return CurrentSelectPort;
}


QStringList SerialPortBasicSetting_c::getAvailablePortsName()
{
    return SerialPort->getAvailablePortsName();
}

QList<QSerialPortInfo> SerialPortBasicSetting_c::getAvailablePortsInfo()
{
    return SerialPort->getAvailablePortsInfo();
}

QString SerialPortBasicSetting_c::getCurrentSelectBaudRate()
{
    return CurrentSelectBaudRate;
}

QString SerialPortBasicSetting_c::getCurrentSelectDataBits()
{
    return CurrentSelectDataBits;
}

QString SerialPortBasicSetting_c::getCurrentSelectParity()
{
    return CurrentSelectParity;
}

QString SerialPortBasicSetting_c::getCurrentSelectStopBits()
{
    return CurrentSelectStopBits;
}

QString SerialPortBasicSetting_c::getCurrentSelectFlowControl()
{
    return CurrentFlowControl;
}


mySerialPort::Port_Status_e SerialPortBasicSetting_c::OpenPort()
{
    qint32 baud_rate = CurrentSelectBaudRate.toInt();

    QSerialPort::DataBits data_bits;
    if(CurrentSelectDataBits == "5"){
        data_bits = QSerialPort::Data5;
    }else if (CurrentSelectDataBits == "6") {
        data_bits = QSerialPort::Data6;
    }else if (CurrentSelectDataBits == "7") {
        data_bits = QSerialPort::Data7;
    }else if (CurrentSelectDataBits == "8") {
        data_bits = QSerialPort::Data8;
    }else {
        data_bits = QSerialPort::Data8;// default
    }

    QSerialPort::FlowControl flow_control;
    if(CurrentFlowControl == "NoFlowControl"){
        flow_control = QSerialPort::NoFlowControl;
    }else if (CurrentFlowControl == "HardwareControl") {
        flow_control = QSerialPort::HardwareControl;
    }else if (CurrentFlowControl == "SoftwareControl") {
        flow_control = QSerialPort::SoftwareControl;
    }else{
        flow_control = QSerialPort::NoFlowControl;// default
    }

    QSerialPort::Parity parity;
    if(CurrentSelectParity == "None"){
        parity = QSerialPort::NoParity;
    }else if (CurrentSelectParity == "Even") {
        parity = QSerialPort::EvenParity;
    }else if (CurrentSelectParity == "Odd") {
        parity = QSerialPort::OddParity;
    }else if (CurrentSelectParity == "Space") {
        parity = QSerialPort::SpaceParity;
    }else if (CurrentSelectParity == "Mark") {
        parity = QSerialPort::MarkParity;
    }else{
        parity = QSerialPort::NoParity;// default
    }

    QSerialPort::StopBits stop_bits;
    if(CurrentSelectStopBits == "One"){
        stop_bits = QSerialPort::OneStop;
    }else if (CurrentSelectStopBits == "OneAndHalf") {
        stop_bits = QSerialPort::OneAndHalfStop;
    }else if (CurrentSelectStopBits == "Two") {
        stop_bits = QSerialPort::TwoStop;
    }else{
        stop_bits = QSerialPort::OneStop;// default
    }

    return SerialPort->openPort(CurrentSelectPort,baud_rate,data_bits,parity,stop_bits,flow_control);
}

void SerialPortBasicSetting_c::ClosePort()
{
    SerialPort->Close();
}
