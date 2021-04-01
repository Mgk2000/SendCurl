#ifndef HOST_H
#define HOST_H

#include <QFileInfo>
#include <QString>
class MainWindow;
class Task;

class Host
{
public:
    Host(MainWindow* mw, const QString& _name);
    QString name;
    virtual void startTask(int itask) = 0;
    virtual QString getResultUrl(const QString& resFile) =0;
    void addTask(QFileInfo* fi, bool run);
    QList <Task*> tasks;
    MainWindow* mainWin;

};
class CurlHost : public Host
{
public:
    CurlHost(MainWindow* mw, const QString& _name, const QString& _url);
    QString url;
    virtual void startTask(int itask);
    virtual QString getResultUrl(const QString& resFile);

};

#endif // HOST_H
