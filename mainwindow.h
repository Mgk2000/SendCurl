#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QTableWidget>
#include <QTableView>
#include <QFileInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class Host;
class Task;
class TaskTable;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QProcess process;

    void send(Host & host, const QString& fn);
    QList <Host*> hosts;
    QString curl;
    QTableWidget* filesTable;
    TaskTable* taskTable;
    QList <QFileInfo*> files;
    QList <Task*> taskList;
    void fillTaskList();
    void addFiles();
    void showFiles();
    void showTasks();
    QString lastDir;

private slots:
    void on_addButton_clicked();
    void filesContextMenu(const QPoint& pos);
    void onTaskChanged();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
