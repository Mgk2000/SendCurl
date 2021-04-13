#include "host.h"
#include "task.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include "mainwindow.h"
#include <QClipboard>
#include <QApplication>
#include <QDir>
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

void Host::runNextTask()
{
    for (int i=0; i< tasks.count(); i++)
        if (tasks[i]->state == waiting)
        {
            startTask(i);
            return;
        }
}

bool Host::removeTask(Task *task)
{
    for (int i =0; i< tasks.count(); i++)
        if (tasks[i] == task)
        {
            tasks[i]->stop();
            tasks.removeAt(i);
            return true;
        }
    return false;
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
       if (mainWin->sendViaTor())
       {
       arguments << "--socks5-hostname";
        arguments << "localhost:9150";
       }
       /*
       arguments << "-F";
       arguments << QString("file=@\"%1\"").arg(dir+'\\'+fi->fileName());
       arguments << "--output";
       task->outFile = "\"" + dir + "\\" + fi->fileName() +"_" + name + ".json\"";
       task->outFile =  dir + "\\" + fi->fileName() +"_" + name + ".json";

       arguments <<  task->outFile;
       arguments << url;
       QString cmd = mainWin->curl;
       for (int i=0; i< arguments.count(); i++)
           cmd += " " + arguments[i];
       QApplication::clipboard()->setText(cmd);
//       task->process.start(cmd);
*/
       QDir::setCurrent(dir);
       arguments << "-F";
       arguments << QString("file=@\"%1\"").arg(fi->fileName());
       arguments << "--output";
       task->outFile = fi->fileName() +"_" + name + ".json";

       arguments <<  task->outFile;
       arguments << url;
       QString cmd = mainWin->curl;
       for (int i=0; i< arguments.count(); i++)
           cmd += " " + arguments[i];
       QApplication::clipboard()->setText(cmd);
//       task->process.start(cmd);

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
    return getUrlFromAnswer(json_string);
}

QString CurlHost::getUrlFromAnswer(const QString &s)
{
    QByteArray json_bytes = s.toLocal8Bit();
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



QString TusHost::getUrlFromAnswer(const QString &s)
{
    int ind1 = s.indexOf('\"', 0);
    ind1 = s.indexOf('\"', ind1+1);
    ind1 = s.indexOf('\"', ind1+1);
    int ind2 = s.indexOf('\"', ind1+1);
    return "https://tusfiles.com/" + s.mid(ind1+1, ind2-ind1-1);

}

QString SolidHost::getUrlFromAnswer(const QString &s)
{
    return "https://solidfiles.com/v/" + s;
}

QString PutreHost::getUrlFromAnswer(const QString &s)
{
    QByteArray json_bytes = s.toLocal8Bit();
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
    bool status = json_obj["status"].toString() =="success";
    QString res;
    if (status)
        res = json_obj["data"].toObject()["link"].toString();
    else
        res = json_obj["message"].toString();
    return res;

}
