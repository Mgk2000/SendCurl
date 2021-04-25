#ifndef TASK_H
#define TASK_H
class Host;
class QFileInfo;
#include <QProcess>
enum TaskState
{
    waiting, starting, running, failed, finished
};

class Task: public QObject
{
    Q_OBJECT
public:
    Task(Host * _host, QFileInfo* _file);
    virtual ~Task() {}
    Host * host;
    void run();
    void stop();
    QFileInfo * finfo;
    QProcess process;
    TaskState state;
    bool isRunning();
    void changed();
    QString standardStr, errorStr;
    QString outFile;
    QString fileUrl;
    bool viaTor;
private slots:

    void onProcessStandardOutput();
    void onProcessErrorOutput();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onError(QProcess::ProcessError error);
signals:
    void taskChanged();
};

#endif // TASK_H
