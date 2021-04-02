#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileInfo>
#include <QProcess>
#include <QDebug>
#include <QFileDialog>
#include <QHeaderView>
#include <QSplitter>
#include "host.h"
#include "taskmodel.h"
#include "tasktable.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    filesTable = new QTableWidget;
    filesTable->verticalHeader()->hide();
    filesTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    filesTable->verticalHeader()->setDefaultSectionSize(16);
    filesTable->horizontalHeader()->hide();
    filesTable->setShowGrid(false);

    filesTable->setColumnCount(1);
    filesTable->setColumnWidth(0,2000);
    filesTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect (filesTable, &QTableWidget::customContextMenuRequested, this, &MainWindow::filesContextMenu);
    //ui->verticalLayout->insertWidget(0, filesTable, 1);
    QSplitter *splitter = new QSplitter(Qt::Vertical);
    taskTable = new TaskTable(this);
    taskTable->setModel(new TaskModel(this));
    //ui->verticalLayout->insertWidget(1, taskTree, 2);
    splitter->addWidget(filesTable);
    splitter->addWidget(taskTable);
    ui->verticalLayout->insertWidget(0,splitter);
    lastDir = "U:/Rindex";
    lastDir = "U:/save";


    curl = "c:\\windows\\system32\\curl.exe";
    hosts.append(new CurlHost(this, "bayfiles", "https://api.bayfiles.com/upload"));
    hosts.append(new CurlHost(this, "bayfiles keyed", "https://api.bayfiles.com/upload?token=4f9a3566f6777bee"));
//    hosts.append(new CurlHost("bayfiles", "https://api.bayfiles.com/upload?token=4f9a3566f6777bee"));
    hosts.append(new CurlHost(this, "anonfiles", "https://api.anonfiles.com/upload"));
    hosts.append(new CurlHost(this, "anonfiles keyed", "https://api.anonfiles.com/upload?token=65919e88def53ba5"));
    hosts.append(new CurlHost(this, "fileleaks", "https://api.fileleaks.com/upload"));
    hosts.append(new CurlHost(this, "fileleaks keyed", "https://api.fileleaks.com/upload?token=72083f7f2aefa10a"));
//    hosts.append(new Host("anonfiles", "https://tusfiles.com/api/upload/server?key=31449212xdl45x5xip46qs7"));
    hosts.append(new TusHost(this,"tusfiles", "https://wwcloud.tusfiles.com/cgi-bin/upload.cgi"));
//    hosts.append(new CurlHost(this,"1fichier", "https://api.1fichier.com/v1/upload/get_upload_server.cgi"));
    hosts.append(new SolidHost(this,"solidfiles", "https://www.solidfiles.com/upload/process/0/"));
    //hosts.append(new CurlHost(this, "iobb", "http://intel.iobb.net/upload.cgi"));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::send(Host &host, const QString& fn)
{
   //QString s = QString(sendstr).arg(fn);
   //curl -F "file=@ph39.rar" https://api.bayfiles.com/upload > bayfiles.res
/*    QFileInfo fi (fn);
    qDebug() << "curl exists=" << QFileInfo::exists(fn);
    qDebug() << "fn exists=" << fi.exists();
   QString dir = fi.canonicalPath();
   dir = dir.replace('/', '\\');
   QStringList arguments;
   arguments << "--verbose";
   arguments << "-F";
   arguments << QString("file=@%1").arg(fn);
   arguments << "--output";
   arguments <<  dir + "\\" + fi.fileName() +"_" + host.name + ".json";
   arguments << host.url;
   host.process.startDetached(curl, arguments);
   qDebug() << host.process.state();*/

}

void MainWindow::fillTaskList()
{
    taskList.clear();
    for (int i =0; i< hosts.count(); i++)
        taskList << hosts[i]->tasks;
    showTasks();
}

void MainWindow::removeTask(int itask)
{
    Task* task = taskList[itask];
    for (int i = 0; i< hosts.count(); i++)
        if (hosts[i]->removeTask(task))
        {
            fillTaskList();
            return;
        }
}

void MainWindow::addFiles()
{
    QFileDialog fd;
    fd.setDirectory(lastDir);
    QStringList ls = fd.getOpenFileNames();
    if (ls.count() ==0)
        return;
    lastDir = QFileInfo(ls[0]).absolutePath();
    qDebug() << ls;
    for (int i=0; i< ls.count(); i++)
    {
        for (int j=0; j< files.count(); j++)
        if (files[j]->absoluteFilePath() == ls[i])
            goto nextfile;
        files.append(new QFileInfo(ls[i]));
        nextfile: ;
    }
    showFiles();
}

void MainWindow::showFiles()
{
    filesTable->clear();
    filesTable->setRowCount(files.count());
    for (int i=0; i< files.count(); i++)
    {
        //filesTable->setRowHeight(i,16);

        qDebug() << files[i]->fileName();
        QTableWidgetItem* item = new QTableWidgetItem(files[i]->fileName());
        item->setData(Qt::UserRole, (quint64)files[i]);

        QFont font = item->font();
        font.setPixelSize(12);
        item->setFont(font);
        filesTable->setItem(i,0, item);
    }

}

void MainWindow::showTasks()
{
    taskTable->taskModel()->refresh();

}

bool MainWindow::sendViaTor() const
{
    return ui->torCheckBox->isChecked();
}


void MainWindow::on_addButton_clicked()
{
//    send(*hosts[1], "u:\\save\\1.rar");
//    send(*hosts[0], "u:\\save\\1.rar");
    addFiles();
}

void MainWindow::filesContextMenu(const QPoint& pos)
{
    QTableWidgetItem *item = filesTable->itemAt(pos);
    if (!item)
        return;
    QFileInfo * fi = (QFileInfo *) item->data(Qt::UserRole).toLongLong();
//        qDebug() << fi->absoluteFilePath();
        // do what you want with the item.
    QList <QAction*> actions;
    QMenu menu;
    for (int i =0; i< hosts.count(); i++)
    {
        QMenu* hostMenu = new QMenu(hosts[i]->name);
        QAction*  startAct = new QAction("Run");
        actions.append(startAct);
        hostMenu->addAction(startAct);
        QAction* enqueueAct = new QAction("Enqueue");
        hostMenu->addAction(enqueueAct);
        actions.append(enqueueAct);
        menu.addMenu(hostMenu);
    }
    QAction* act = menu.exec(mapToGlobal(pos));
    if (!act)
        return;
    for (int i =0; i< hosts.count(); i++)
    {
        if (act == actions[i*2])
        {
            hosts[i]->addTask(fi, true);
            break;
        }
        else if (act == actions[i*2+1])
        {
            hosts[i]->addTask(fi, false);
            break;
        }
    }
    fillTaskList();


}

void MainWindow::onTaskChanged()
{
    showTasks();

}
