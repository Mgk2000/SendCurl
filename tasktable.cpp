#include "tasktable.h"
#include "task.h"
#include "mainwindow.h"
#include <QDebug>
#include <QMenu>
#include <QMouseEvent>
#include <QClipboard>
#include <QApplication>

TaskTable::TaskTable(MainWindow *mw): mainWin(mw)
{

}

void TaskTable::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << event;
    QPoint pos = event->pos();
    QPersistentModelIndex index = indexAt(pos);
    qDebug() << index.row() << index.column();
    if (index.row() <0 || index.column() <0)
        return;
//    qInfo() << model.files[index.row()]->name;
    if(event->button() == Qt::RightButton)
    {
        if(index.row() >=0)
            contextMenu(index.row(), pos);
        //emit activated(index);
    }
    else
        QTableView::mouseReleaseEvent(event);
}

void TaskTable::contextMenu(int row, const QPoint &pos)
{
    QMenu menu;
    QAction stopAct("Stop");
    QAction startAct("Start");
    QAction removeAct("Remove");
    QAction copyAct("Copy Url");
    Task * task = mainWin->taskList[row];
    if (task->isRunning())
        menu.addAction(&stopAct);
    else
        menu.addAction(&startAct);
    if (task->state == finished)
        menu.addAction(&copyAct);
    menu.addAction(&removeAct);
    QAction * act = menu.exec(mapToGlobal(pos));
    if (act == &stopAct)
        task->stop();
    else if (act == &startAct)
        task->run();
    else if (act == &removeAct)
        mainWin->removeTask(row);
    else if (act == &copyAct)
        QApplication::clipboard()->setText(task->fileUrl);
}

TaskModel *TaskTable::taskModel()
{
    return (TaskModel*) model();
}
