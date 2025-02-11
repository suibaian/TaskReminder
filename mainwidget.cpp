#include "mainwidget.h"
#include "./ui_mainwidget.h"
#include <iostream>

#include "adddialog.h"
#include "common.h"

#include <QDebug>
#include <qfile.h>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "taskwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    if( this->loadTasksFromFile())
    {
        qDebug() << "Tasks loaded from file!";
    }
    else
    {
        qDebug() << "Failed to load tasks from file!";
    }

    QStringList mainCategoryList = categoryList;
    mainCategoryList.prepend("All");
    ui->class_comboBox->addItems(priorityList);
    ui->sort_comboBox->addItems(mainCategoryList);

    ui->class_comboBox->setCurrentIndex(0);
    ui->sort_comboBox->setCurrentIndex(1);

    // 连接信号和槽

    // 连接 addTask_pushButton 的 clicked 信号和 on_addTask_pushButton_clicked 槽函数
    connect(ui->addTask_pushButton, &QPushButton::clicked, this, &MainWidget::createNewTask);
}

MainWidget::~MainWidget()
{
    delete ui;

}

// 排序任务--ai
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

// 筛选任务--ai
QList<Task> MainWidget::filterTasks(const QList<Task>& tasks, const QString& category) {
    QList<Task> filteredTasks;
    for (const Task& task : tasks) {
        if (category == "All" || task.category == category) {
            filteredTasks.append(task);
        }
    }
    return filteredTasks;
}


void MainWidget::saveTasksToFile(const QList<Task>& tasks) {

    foreach (const Task &task, tasks) {
        QJsonObject taskObject;
        taskObject["content"] = task.content;
        taskObject["priority"] = task.priority;
        taskObject["category"] = task.category;
        taskObject["isCompleted"] = task.isCompleted;
        taskObject["date"] = task.date.toString("yyyy-MM-dd");
    
        QJsonDocument doc(taskObject);
        QFile file("task.json");
    
        if (file.open(QIODevice::Append)) {
            file.write(doc.toJson());
            file.close();
            qDebug() << "Task saved to file!";
        } else {
            qDebug() << "Failed to open file for writing!";
        }
    }
}


bool MainWidget::loadTasksFromFile() {
    /*
    QFile file("task.json");

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file for reading!";
        return false;  // Return an empty task or handle it as needed
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QJsonDocument doc = QJsonDocument::fromJson(line.toUtf8());
    
        if (!doc.isObject()) {
            qDebug() << "Invalid JSON format!";
            return false;  // Return an empty task or handle it as needed
        }
    
        QJsonObject taskObject = doc.object();
    
        Task task;
        task.content = taskObject["content"].toString();
        task.priority = taskObject["priority"].toInt();
        task.category = taskObject["category"].toString();
        task.isCompleted = taskObject["isCompleted"].toBool();
        task.date = QDate::fromString(taskObject["date"].toString(), "yyyy-MM-dd");
    
        qDebug() << "Task loaded from file!";
        this->tasks.append(task);
    }
    file.close();
    return true;
    */

    QFile file("tasks.json");
    
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file!";
        return false;
    }
    
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    
    if (!doc.isArray()) {
        qDebug() << "Invalid JSON format!";
        return false;
    }
    
    QJsonArray taskArray = doc.array();
    
    for (const QJsonValue& value : taskArray) {
        if (!value.isObject()) {
            continue;
        }
        
        QJsonObject taskObject = value.toObject();
        
        QString category = taskObject["category"].toString();
        QString content = taskObject["content"].toString();
        QDate date = QDate::fromString(taskObject["date"].toString(), "yyyy-MM-dd");
        bool isCompleted = taskObject["isCompleted"].toBool();
        int priority = taskObject["priority"].toInt();
        
        Task task(category, content, date, isCompleted, priority);
        tasks.append(task);  // 将Task对象添加到列表中
    }

    qDebug() << "Tasks loaded: " << tasks.size();
    return true;
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

    tasks.append(task);

    // 将任务添加到任务列表
    // 任务列表是一个 QJsonArray
    // QJsonObject taskObject;
    // taskObject["content"] = task.content;
    // taskObject["priority"] = task.priority;
    // taskObject["category"] = task.category;
    // taskObject["isCompleted"] = task.isCompleted;
    // taskObject["date"] = task.date.toString("yyyy-MM-dd");
    // qDebug() << "Task added: " << taskObject;

    // 将 QWidget 添加到 QListWidget
    QListWidgetItem *item = new QListWidgetItem();
    ui->task_listWidget->addItem(item);


    // 创建自定义控件 TaskWidget
    TaskWidget *taskWidget = new TaskWidget(task);
    ui->task_listWidget->setItemWidget(item, taskWidget);  // 将控件放入 item 中
    item->setSizeHint(QSize(1000, 200));
    ui->task_listWidget->setCurrentItem(item);

    // 保存任务到文件
    this->saveTasksToFile(tasks);
}
