#include "qlibfile.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>

bool FileLib::RemovePath(const QString dirname)
{
    bool ret = true;
    QDir dir(dirname);

    if(dir.exists())
    {
        QFileInfoList infolist = dir.entryInfoList();

        for(int i = 0; i < infolist.count(); i++)
        {
            QFileInfo info = infolist.at(i);
            if((info.fileName() == ".") || (info.fileName() == ".."))continue;

            if(info.isDir())
            {
                ret = RemovePath(info.absoluteFilePath());
            }
            else
            {
                ret = QFile::remove(info.absoluteFilePath());
            }

            if(!ret)
            {
                return ret;
            }
        }
        ret = dir.rmdir(dirname);
    }
    return ret;
}

bool FileLib::RemoveFile(const QString firname)
{
    return QFile::remove(firname);
}

void FileLib::CheckPath(const QString dirname)
{
    QStringList dirpath = dirname.split("//");
    QString newdirname = "";

    for(int i = 0;i<dirpath.count();i++)
    {
        for(int j = 0; j< dirpath[i].count();j++)
        {
            if((dirpath[i].at(j) == '.')&&
                    (dirpath[i].compare("."))&&
                    (dirpath[i].compare("..")))
            {
                goto process;
            }
        }
        newdirname.append(dirpath[i]).append("//");
    }

    process:
    qDebug() << newdirname;

    QDir dir(newdirname);
    if(!dir.exists())
    {
        bool ismkdir = dir.mkpath(".//");

        if(!ismkdir)
        {
            qDebug() << "Create path fail" << Qt::endl;
        }
        else
        {
            qDebug() << "Create path success" << Qt::endl;
        }
    }
    else
    {
        qDebug() << "full path exist" << Qt::endl;
    }
}

bool FileLib::CopyFile(QString oldname, QString newname)
{
    if(oldname == newname)
    {
        qDebug() << "File is Same";
        return false;
    }

    CheckPath(newname);

    return QFile::copy(oldname,newname);
}

