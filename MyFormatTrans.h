#ifndef MYFORMATTRANS_H
#define MYFORMATTRANS_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QChar>

namespace  MyFormatTrans{
    QString QCharTOQStringHex(QChar input);

    QChar HexDataConvert(int input);
    int HexDataConvert(QChar input);

    QChar QStringTOQCharHex(QString input);

    qint64 TimeTransToMicroSecond(QString input);
    qint64 TimeIntervalInMicroSecond(QString old_time, QString new_time);

}



#endif // MYFORMATTRANS_H
