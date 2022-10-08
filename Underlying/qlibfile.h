#ifndef QLIBFILE_H
#define QLIBFILE_H

#include <QObject>
#include <QString>


namespace FileLib {
    bool RemovePath(const QString dirname);
    bool RemoveFile(const QString firname);
    void CheckPath(const QString dirname);
    bool CopyFile(QString oldname, QString newname);
}


#endif // QLIBFILE_H
