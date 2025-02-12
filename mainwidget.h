#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>


#include "adddialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWidget;
}
QT_END_NAMESPACE

// class Task;


class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    void sortTasks(QList<Task> &tasks, const QString &sortBy);

    QList<Task> filterTasks(const QList<Task> &tasks, const QString &category);

    void saveTasksToFile(const QList<Task> &tasks);

    bool loadTasksFromFile();

    // update task list widget 
    void updateTaskListWidget(const QList<Task> &tasks);

    // add single task to list
    void addTask(const Task &task);


private slots:
    // open add task dialog
    void on_createNewTask_Dialog();

    // on AddDialog taskAdded signal, need to add task to list
    void on_addTask_handled(const Task &task);

    // using id to delete task from list
    void on_deleteTask_handled(int taskId);

private:
    Ui::MainWidget *ui;
    AddDialog *addDialog;  // 添加任务的对话框
    QList<Task> tasks;  // 存储任务的容器
    QString filePath = "tasks.txt";
};
#endif // MAINWIDGET_H
