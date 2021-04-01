#include "tasktable.h"
#include "mainwindow.h"

TaskTable::TaskTable(MainWindow *mw): mainWin(mw)
{

}

TaskModel *TaskTable::taskModel()
{
    return (TaskModel*) model();
}
