#include "MyFormatTrans.h"

#include <QDebug>

QString MyFormatTrans::QCharTOQStringHex(QChar input)
{
    uchar input_char = input.toLatin1();

    return QString("").append(HexDataConvert((input_char/16)%16)).append(HexDataConvert(input_char%16));
}

QChar MyFormatTrans::HexDataConvert(int input)
{
    if (input <= 9){
        return QChar('0' + input);
    } else {
        return QChar('A' + input - 10);
    }
}

int MyFormatTrans::HexDataConvert(QChar input)
{
    if((input <= '9')&&(input >= '0'))
    {
        return input.toLatin1() - '0';
    } else if((input <= 'F')&&(input >= 'A')){
        return input.toLatin1() - 'A' + 10;
    } else if ((input <= 'f')&&(input >= 'a')) {
        return input.toLatin1() - 'a' + 10;
    }
    return -1;
}

QChar MyFormatTrans::QStringTOQCharHex(QString input)
{
    if(input.count()!= 2){
        return QChar(0);
    }

    int char_in_int = HexDataConvert(input.at(0))*16 + HexDataConvert(input.at(1));

    return QChar(char_in_int);
}


/*
 * @brief   :   Transfer time string to ms
 * @param   :   input   :   the time string
 *                      1. Format   :   "hour:miniuts:second:microsecond"
 * @return  :   the value in microsecond
 */
qint64 MyFormatTrans::TimeTransToMicroSecond(QString input)
{
    QStringList time_split = input.split(":");

    qint64 time_in_ms = 0;

    for(int i = 0;i < time_split.count() - 1; i++)
    {
        time_in_ms *= 60;
        time_in_ms += time_split.at(i).toInt();
    }

    time_in_ms *= 1000;
    time_in_ms += time_split.at(time_split.count() - 1).toInt();


    return time_in_ms;
}


qint64 MyFormatTrans::TimeIntervalInMicroSecond(QString old_time, QString new_time)
{
    QStringList old_time_split = old_time.split(":");
    QStringList new_time_split = new_time.split(":");

    int number_of_time = old_time_split.count();
    if(number_of_time < new_time_split.count())
    {
        number_of_time = new_time_split.count();
    }

    qint64 time_in_ms = 0;



    for(int i = 0;i < number_of_time - 1; i++)
    {
        time_in_ms *= 60;
        time_in_ms += new_time_split.at(i).toInt() - old_time_split.at(i).toInt();
    }

    time_in_ms *= 1000;
    time_in_ms += new_time_split.at(number_of_time - 1).toInt() - old_time_split.at(number_of_time - 1).toInt();

    return time_in_ms;
}
