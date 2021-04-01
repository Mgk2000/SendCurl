#include "taskmodel.h"
#include "mainwindow.h"
#include "task.h"
#include "host.h"

TaskModel::TaskModel(MainWindow *mw) : mainWin(mw)
{

}

int TaskModel::rowCount(const QModelIndex &parent) const
{
    return mainWin->taskList.count();
}

void TaskModel::refresh()
{
    QModelIndex topLeft = index(0, 0);
       QModelIndex bottomRight = index(mainWin->taskList.count(), 2);

       emit dataChanged(topLeft, bottomRight);
       emit layoutChanged();
}

QVariant TaskModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int r = index.row();
    int c = index.column();
    Task * task = mainWin->taskList[r];
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch (c)
        {
        case 0:
            return task->host->name;
        case 1:
            return task->finfo->fileName();
        case 2:
            {
                switch (task->state)
                {
                case TaskState::waiting:
                    return "Waiting";
                case TaskState::starting:
                    return "Starting";
                case TaskState::running:
                    return task->standardStr;
                case TaskState::failed:
                    return task->errorStr;
                case TaskState::finished:
                    return task->fileUrl;
                }
            }
        }
    }
    else if(role ==  Qt::ForegroundRole)
   {
        switch (task->state)
        {
        case TaskState::waiting:
            return QColor(Qt::black);
        case TaskState::starting:
            return QColor(Qt::darkCyan);
        case TaskState::running:
            return QColor(Qt::blue);
        case TaskState::failed:
            return QColor(Qt::red);
        case TaskState::finished:
            return QColor(Qt::green);
        }
   }


    return QVariant();

}

Qt::ItemFlags TaskModel::flags(const QModelIndex &index) const
{
    if (index.column() == 2)
        return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
    else
        return QAbstractTableModel::flags(index);

}
