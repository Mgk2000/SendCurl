#ifndef TASKMODEL_H
#define TASKMODEL_H
#include <QAbstractTableModel>

class MainWindow;
class TaskModel : public QAbstractTableModel
{
public:
    TaskModel(MainWindow* mw);
    int rowCount(const QModelIndex &parent) const ;
    int columnCount(const QModelIndex &parent) const {return 3;}
    void refresh();
    //QVariant headerData(int section,
     //                Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) {return true;}
    Qt::ItemFlags flags(const QModelIndex &index) const;


    //bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    //Qt::ItemFlags flags(const QModelIndex &index) const;

    MainWindow* mainWin;
};

#endif // TASKMODEL_H
