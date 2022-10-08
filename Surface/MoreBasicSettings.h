#ifndef MOREBASICSETTINGS_H
#define MOREBASICSETTINGS_H

#include <QWidget>
#include <QSerialPort>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTimer>

#ifndef PORT_REFRESH_PERIOD
#define PORT_REFRESH_PERIOD 1000//ms
#endif

#ifndef COUNTER_UPDATE_PERIOD
#define COUNTER_UPDATE_PERIOD 500//ms
#endif

namespace Ui {
class MoreBasicSettings;
}

class MoreBasicSettings : public QWidget
{
    Q_OBJECT

public:
    explicit MoreBasicSettings(QWidget *parent = nullptr);

    void Init();

    ~MoreBasicSettings();

signals:
    void MoreBasicSettingOpenPort();


private slots:
    void on_BaudRate_LineEdit_textChanged(const QString &arg1);

    void on_DataBits_Combo_currentTextChanged(const QString &arg1);

    void on_Port_Combo_currentTextChanged(const QString &arg1);

    void on_Parity_Combo_currentTextChanged(const QString &arg1);

    void on_FlowControl_Combo_currentTextChanged(const QString &arg1);

    void on_StopBits_Combo_currentTextChanged(const QString &arg1);

    void on_Cancel_PushButton_clicked();

    void on_PortOpenClose_PushButton_clicked();

    void on_Port_Refresh_clicked();



    void on_PortDetail_TreeWidget_itemClicked(QTreeWidgetItem *item, int column);

private:
    QList<QSerialPortInfo> available_port_info;

    QTimer *Timer;

    Ui::MoreBasicSettings *ui;
};

#endif // MOREBASICSETTINGS_H
