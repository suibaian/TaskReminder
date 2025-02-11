#include "mainwidget.h"
#include "./ui_mainwidget.h"
#include <iostream>

#include "addDailog.h"

#include <QDebug>
#include <qfile.h>
#include "taskwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    // 连接信号和槽

    // 连接 addTask_pushButton 的 clicked 信号和 on_addTask_pushButton_clicked 槽函数
    connect(ui->addTask_pushButton, &QPushButton::clicked, this, &MainWidget::createNewTask);
}

MainWidget::~MainWidget()
{
    delete ui;

}

// 排序任务
void MainWidget::sortTasks(QList<Task>& tasks, const QString& sortBy) {
    if (sortBy == "Date") {
        std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
            return a.date < b.date;
        });
    } else if (sortBy == "Priority") {
        std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
            return a.priority < b.priority;
        });
    }
}

// 筛选任务
QList<Task> MainWidget::filterTasks(const QList<Task>& tasks, const QString& category) {
    QList<Task> filteredTasks;
    for (const Task& task : tasks) {
        if (category == "All" || task.category == category) {
            filteredTasks.append(task);
        }
    }
    return filteredTasks;
}



void MainWidget::saveTasksToFile(const QList<Task>& tasks, const QString& filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const Task& task : tasks) {
            out << task.content << "," << task.priority << "," << task.category << "," << task.isCompleted << "," << task.date.toString() << "\n";
        }
    }
}

QList<Task> MainWidget::loadTasksFromFile(const QString& filePath) {
    QList<Task> tasks;
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(",");
            if (fields.size() == 5) {
                Task task;
                task.content = fields[0];
                task.priority = fields[1];
                task.category = fields[2];
                task.isCompleted = fields[3] == "1";  // assuming 1 is true for completed
                task.date = QDate::fromString(fields[4]);
                tasks.append(task);
            }
        }
    }
    return tasks;
}


// 槽函数
void MainWidget::createNewTask()
{
    // qDebug() << "Add Task button clicked";
    std::cout << "Add Task button clicked" << std::endl;

    // 创建一个对话框
    AddDialog *addDialog = new AddDialog(this);
    // 连接 AddDialog 的 taskAdded 信号 到 MainWidget 的 addTask 槽函数
    connect(addDialog, &AddDialog::taskAdded, this, &MainWidget::addTask);

    addDialog->exec();  // 显示对话框，并阻塞主线程，直到对话框关闭
}

// 槽函数
void MainWidget::addTask(const Task &task)
{
    // ToDo: 添加任务到任务列表

    // qDebug() << "Task added";
    std::cout << "Task added" << std::endl;

    // 将任务添加到任务列表
    // 任务列表是一个 QJsonArray
    QJsonObject taskObject;
    taskObject["content"] = task.content;
    taskObject["priority"] = task.priority;
    taskObject["category"] = task.category;
    taskObject["isCompleted"] = task.isCompleted;
    taskObject["date"] = task.date.toString("yyyy-MM-dd");

    qDebug() << "Task added: " << taskObject;

    // 创建自定义控件 TaskWidget
    TaskWidget *taskWidget = new TaskWidget(task);

    // 将 QWidget 添加到 QListWidget
    QListWidgetItem *item = new QListWidgetItem();
    ui->task_listWidget->addItem(item);
    ui->task_listWidget->setItemWidget(item, taskWidget);  // 将控件放入 item 中


}