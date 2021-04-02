#include "task.h"
#include "host.h"
#include <QFileInfo>
#include <QDebug>
#include "mainwindow.h"

Task::Task(Host *_host, QFileInfo *_file) : host(_host), finfo(_file)
{
    qDebug() << "Added task " << host->name <<  finfo->absoluteFilePath();
//    host->addTask(_file);
    connect(&process, SIGNAL(readyReadStandardOutput()), this, SLOT(onProcessStandardOutput()));
    connect(&process, SIGNAL(readyReadStandardError()), this, SLOT(onProcessErrorOutput()));
    connect(&process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(onProcessFinished(int,QProcess::ExitStatus)));
    connect (this, SIGNAL(taskChanged()), host->mainWin, SLOT(onTaskChanged()));
    state = waiting;

}

void Task::run()
{
    qDebug() << "Started task " << host->name <<  finfo->absoluteFilePath();
    int npr =-1;
    for (int i=0; i< host->tasks.count(); i++)
        if (host->tasks[i] == this)
        {
            npr = i;
            break;
        }
    if (npr != -1)
    {
        host->startTask(npr);
        state = starting;
        changed();
    }
}

void Task::stop()
{
    process.close();
    state = waiting;
    changed();
}

bool Task::isRunning()
{
    return state == starting || state == running;
}

void Task::changed()
{
    emit taskChanged();
}

void Task::onProcessStandardOutput()
{
    QByteArray ba = process.readAllStandardOutput();
    standardStr = QString(ba);
    qInfo() << "StOut" << standardStr;
    state = running;
    changed();
}

void Task::onProcessErrorOutput()
{
    QByteArray ba = process.readAllStandardError();
    standardStr = QString(ba);
    qDebug() << "==Error=="<< standardStr;
    state = running;
    changed();

}

void Task::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qInfo() << "Process finished exitCode=" << exitCode << " exitStatus=" << exitStatus;
    if (exitStatus==QProcess::NormalExit && exitCode == 0)
    {
        fileUrl  = host->getResultUrl(outFile);
        state = finished;
        host->runNextTask();
    }
    else
    {
        errorStr = QString("Failed exitcode=%1").arg(exitCode);
        state = failed;
    }
    changed();
}

void Task::onError(QProcess::ProcessError error)
{
    state = failed;
    errorStr = QString ("Error %1").arg(error);
    changed();

}


