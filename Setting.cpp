#include "Setting.h"
#include <QDebug>
#include <QTextCodec>
#include <Underlying/qlibfile.h>

Setting_c Settings;

Setting_c::Setting_c()
{

}

void Setting_c::Init()
{
    Setting = new QSettings(SettingPath,QSettings::IniFormat);
    ATCommandList = new QSettings(ATCommandListPath,QSettings::IniFormat);
}


void Setting_c::setBasicSettingValue(const QString &key, const QVariant &value)
{
    Setting->setValue(QString("BasicSetting/").append(key),value);
    Setting->sync();
}


QVariant Setting_c::getBasicSettingValue(const QString &key, const QVariant &defaultValue) const
{
    return Setting->value(QString("BasicSetting/").append(key),defaultValue);
}

void Setting_c::setSendReceiveAreaValue(const QString &key, const QVariant &value)
{
    Setting->setValue(QString("SendReceiveArea/").append(key),value);
    Setting->sync();
}


QVariant Setting_c::getSendReceiveAreaValue(const QString &key, const QVariant &defaultValue) const
{
    return Setting->value(QString("SendReceiveArea/").append(key),defaultValue);
}



void Setting_c::addATCommand(const QString &command,const QString &category,const QString &discription)
{
    QString command_name = category;
    command_name = command_name.append("/").append(command);
    ATCommandList->setValue(command_name,discription);
}

void Setting_c::deleteATCommand(const QString &command,const QString &category)
{
    ATCommandList->beginGroup(category);
    ATCommandList->remove(command);
    ATCommandList->endGroup();
}

QStringList Setting_c::getATCommandList()
{
    return ATCommandList->allKeys();
}

QStringList Setting_c::getATCommandList(const QString &category)
{
    ATCommandList->beginGroup(category);
    QStringList childkeys = ATCommandList->childKeys();
    ATCommandList->endGroup();
    return childkeys;

}


QStringList Setting_c::getATCommandCategorys()
{
    return ATCommandList->childGroups();
}



QString Setting_c::getATCommandDiscription(const QString &command, const QString &category)
{
    QString command_name = category;
    command_name = command_name.append("/").append(command);
    return ATCommandList->value(command_name,"None").toString();
}


bool Setting_c::importSettingFile(Configuration document, QString file_path)
{
    QString new_file_name;
    switch (document) {
    case BasicSettings:
    case SendReceiveAreaSettings:
        new_file_name = SettingPath;
        break;
    case ATSettings:
        new_file_name = ATCommandListPath;
        delete ATCommandList;
        break;
    default:
        break;
    }
    file_path.replace("/","//");
    new_file_name.replace("/","//");


    qDebug() << "at path:\t" << new_file_name;
    qDebug() << "select path:\t" << file_path;

    FileLib::RemovePath(new_file_name);
    return FileLib::CopyFile(file_path,new_file_name);
}


bool Setting_c::exportSettingFile(Configuration document, QString file_path)
{
    QString old_file_name;
    switch (document) {
    case BasicSettings:
    case SendReceiveAreaSettings:
        old_file_name = SettingPath;
        break;
    case ATSettings:
        old_file_name = ATCommandListPath;
        break;
    default:
        break;
    }

    file_path.append(".ini");
    file_path.replace("/","//");
    old_file_name.replace("/","//");
    return FileLib::CopyFile(old_file_name,file_path);
}


