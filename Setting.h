#ifndef SETTING_C_H
#define SETTING_C_H

#include <QSettings>
#include <QCoreApplication>
#include <QVariant>
#include <QStringList>

#define AppDirPath          QCoreApplication::applicationDirPath()
#define SettingPath         AppDirPath.append("/SerialPortMonitorCustomSetting.ini")
#define ATCommandListPath   AppDirPath.append("/ATCommandList.ini")

class Setting_c : public QObject
{
public:
    enum Configuration{
        BasicSettings = 0,
        SendReceiveAreaSettings = 1,
        ATSettings = 2,
    };

    Setting_c();

    void Init();


    void setBasicSettingValue(const QString &key, const QVariant &value);
    QVariant getBasicSettingValue(const QString &key, const QVariant &defaultValue = QVariant()) const;

    void setSendReceiveAreaValue(const QString &key, const QVariant &value);
    QVariant getSendReceiveAreaValue(const QString &key, const QVariant &defaultValue = QVariant()) const;

    void addATCommand(const QString &command,const QString &category,const QString &discription);
    void deleteATCommand(const QString &command,const QString &category);
    QStringList getATCommandList();
    QStringList getATCommandList(const QString &category);
    QStringList getATCommandCategorys();
    QString getATCommandDiscription(const QString &command, const QString &category);

    bool exportSettingFile(Configuration document, QString file_path);
    bool importSettingFile(Configuration document, QString file_path);


    QSettings *Setting;
    QSettings *ATCommandList;

};

extern Setting_c Settings;

#endif // SETTING_C_H
