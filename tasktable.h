#ifndef TASKTABLE_H
#define TASKTABLE_H

#include <QTableView>
class MainWindow;
class TaskModel;

class TaskTable : public QTableView
{
public:
    TaskTable(MainWindow* mw);

    MainWindow* mainWin;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void contextMenu(int row, const QPoint& pos);

    TaskModel* taskModel();
};

#endif // TASKTABLE_H
