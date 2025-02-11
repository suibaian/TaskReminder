#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>


#include "addDailog.h"

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

private slots:
    void createNewTask();

    void addTask(const Task &task);

    // void on_addTask_pushButton_clicked();

private:
    Ui::MainWidget *ui;
    AddDialog *addDialog;  // 添加任务的对话框
    QList<Task> tasks;  // 存储任务的容器
    QString filePath = "tasks.txt";
};
#endif // MAINWIDGET_H
