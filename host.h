#ifndef HOST_H
#define HOST_H

#include <QFileInfo>
#include <QString>
class MainWindow;
class Task;

class Host //: public QObject
{
    //Q_OBJECT
public:
    Host(MainWindow* mw, const QString& _name);
    virtual ~Host(){}
    QString name;
    virtual void startTask(int itask) = 0;
    virtual QString getResultUrl(const QString& resFile) =0;
    void addTask(QFileInfo* fi, bool run);
    void runNextTask();
    QList <Task*> tasks;
    MainWindow* mainWin;
    bool removeTask(Task* task);
//public slots:
    //void onTaskFinished(Task * task);

};
class CurlHost : public Host
{
public:
    CurlHost(MainWindow* mw, const QString& _name, const QString& _url);
    virtual ~CurlHost(){}

    QString url;
    virtual void startTask(int itask);
    virtual QString getResultUrl(const QString& resFile);
    virtual QString getUrlFromAnswer(const QString & s);
};
class TusHost : public CurlHost
{
public:
    TusHost(MainWindow* mw, const QString& _name, const QString& _url) : CurlHost(mw, _name, _url) {}
    virtual ~TusHost(){}

    virtual QString getUrlFromAnswer(const QString & s);
};
class SolidHost : public CurlHost
{
public:
    SolidHost(MainWindow* mw, const QString& _name, const QString& _url) : CurlHost(mw, _name, _url) {}
    virtual ~SolidHost(){}

    virtual QString getUrlFromAnswer(const QString & s);
};

#endif // HOST_H
