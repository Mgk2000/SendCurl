#include "host.h"
#include "task.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include "mainwindow.h"

Host::Host(MainWindow* mw, const QString &_name) : name(_name), mainWin(mw)
{

}

void Host::addTask(QFileInfo *fi, bool run)
{
    Task * task = new Task(this, fi);
    tasks << task;
    if (run)
        this->startTask(tasks.count() -1);
}

CurlHost::CurlHost(MainWindow* mw, const QString &_name, const QString& _url) : Host(mw, _name), url(_url)
{

}

void CurlHost::startTask(int itask)
{
    Task * task = tasks[itask];
      QFileInfo* fi =task->finfo;
       qDebug() << "fo exists=" << fi->exists();
       QString dir = fi->canonicalPath();
       dir = dir.replace('/', '\\');
       QStringList arguments;
//       arguments << "--verbose";
       arguments << "-F";
       arguments << QString("file=@%1").arg(dir+'\\'+fi->fileName());
       arguments << "--output";
       task->outFile = dir + "\\" + fi->fileName() +"_" + name + ".json";
       arguments <<  task->outFile;
       arguments << url;
       task->process.start(mainWin->curl, arguments);
//       task->process.startDetached(mainWin->curl, arguments);
       qDebug() << task->process.state();

}

QString CurlHost::getResultUrl(const QString &resFile)
{
    QFile f (resFile);
    if (!f.open(QIODevice::ReadOnly))
        return "";
    QTextStream file_text(&f);
    QString json_string;
    json_string = file_text.readAll();
    f.close();
    QByteArray json_bytes = json_string.toLocal8Bit();
    auto json_doc = QJsonDocument::fromJson(json_bytes);

    if (json_doc.isNull()) {
         qDebug() << "Failed to create JSON doc." ;
         return "";
     }
    if (!json_doc.isObject()) {
         qDebug()  << "JSON is not an object.";
         return "";
     }

    QJsonObject json_obj = json_doc.object();

    if (json_obj.isEmpty()) {
         qDebug()  << "JSON object is empty.";
         return "";}
    bool status = json_obj["status"].toBool();
    QString res;
    if (status)
        res = json_obj["data"].toObject()["file"].toObject()["url"].toObject()["full"].toString();
    else
        res = json_obj["msg"].toString();
    return res;
}


