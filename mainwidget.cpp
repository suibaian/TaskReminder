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
// #include <QScroller>
#include <QScrollBar>
// #include <QVScroller>

#include "taskwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    if( this->loadTasksFromFile())
    {
        qDebug() << "Tasks loaded from file!";
        updateTaskListWidget(tasks);
    }
    else
    {
        qDebug() << "Failed to load tasks from file!";
    }

    QStringList mainCategoryList = categoryList;
    mainCategoryList.prepend("All");
    ui->class_comboBox->addItems(mainCategoryList);
    ui->sort_comboBox->addItems({"Date", "Priority"});

    // ui->class_comboBox->setCurrentIndex(0);
    // ui->sort_comboBox->setCurrentIndex(1);

    // 连接信号和槽

    // 连接 addTask_pushButton 的 clicked 信号和 on_addTask_pushButton_clicked 槽函数
    connect(ui->addTask_pushButton, &QPushButton::clicked, this, &MainWidget::on_createNewTask_Dialog);

    // 启用平滑滚动
    // QScroller *scroller = QScroller::scroller(ui->task_listWidget);
    // scroller->grabGesture(ui->task_listWidget, QScroller::TouchGesture);

    // // 调整滚动参数
    // QScrollerProperties properties = scroller->scrollerProperties();
    // properties.setScrollMetric(QScrollerProperties::DecelerationFactor, 0.1);
    // properties.setScrollMetric(QScrollerProperties::MaximumVelocity, 0.5);
    // properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.2);
    // properties.setScrollMetric(QScrollerProperties::OvershootScrollDistanceFactor, 0.2);
    // properties.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.2);
    // scroller->setScrollerProperties(properties);


    ui->task_listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->task_listWidget->verticalScrollBar()->setSingleStep(20); //20一次滚动的距离，自行调整

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
    QJsonArray taskArray;
    foreach (const Task &task, tasks) {
        QJsonObject taskObject;
        taskObject["content"] = task.content;
        taskObject["priority"] = task.priority;
        taskObject["category"] = task.category;
        taskObject["isCompleted"] = task.isCompleted;
        taskObject["date"] = task.date.toString("yyyy-MM-dd");
        taskArray.append(taskObject);
    }

    QJsonDocument doc(taskArray);
    QFile file("tasks.json");

    if (file.open(QIODevice::WriteOnly)) {  // 使用 WriteOnly 模式覆盖文件内容
        file.write(doc.toJson());
        file.close();
        qDebug() << "Tasks saved to file!";
    } else {
        qDebug() << "Failed to open file for writing!";
    }
}


bool MainWidget::loadTasksFromFile() {
    QFile file("tasks.json");
    
    if (!file.exists()) {
        qDebug() << "File does not exist!";
        return false;
    }

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


void MainWidget::updateTaskListWidget(const QList<Task>& tasks)
{
    // 清空任务列表
    ui->task_listWidget->clear();

    // 遍历任务列表
    for (const Task &task : tasks)
    {
        addTask(task);
    }
}


void MainWidget::addTask(const Task &task)
{
    // 将 QWidget 添加到 QListWidget
    QListWidgetItem *item = new QListWidgetItem();
    ui->task_listWidget->addItem(item);


    // 创建自定义控件 TaskWidget
    TaskWidget *taskWidget = new TaskWidget(task);
    ui->task_listWidget->setItemWidget(item, taskWidget);  // 将控件放入 item 中
    item->setSizeHint(QSize(800, 150));
    ui->task_listWidget->setCurrentItem(item);

    // 设置项的背景颜色
    if (ui->task_listWidget->count() % 2 == 0) {
        item->setBackgroundColor(QColor(255, 255, 255));  // 白色
    } else {
        item->setBackgroundColor(QColor(240, 240, 240));  // 浅灰色
    }

    // 连接 TaskWidget 的 deleteTask 信号 到 MainWidget 的 deleteTask 槽函数
    connect(taskWidget, &TaskWidget::deleteTask, this, &MainWidget::on_deleteTask_handled);

}

//******************************************* *槽函数***************************************************

void MainWidget::on_createNewTask_Dialog()
{
    // qDebug() << "Add Task button clicked";
    std::cout << "Add Task button clicked" << std::endl;

    // 创建一个对话框
    AddDialog *addDialog = new AddDialog(this);
    // 连接 AddDialog 的 taskAdded 信号 到 MainWidget 的 addTask 槽函数
    connect(addDialog, &AddDialog::taskAdded, this, &MainWidget::on_addTask_handled);

    addDialog->exec();  // 显示对话框，并阻塞主线程，直到对话框关闭
}

void MainWidget::on_addTask_handled(const Task &task)
{
    // ToDo: 添加任务到任务列表
    tasks.append(task);
    // 更新任务列表
    updateTaskListWidget(tasks);
    // 保存任务到文件
    this->saveTasksToFile(tasks);

    // 将任务添加到任务列表
    // 任务列表里的一个 QJsonArray
    // QJsonObject taskObject;
    // taskObject["content"] = task.content;
    // taskObject["priority"] = task.priority;
    // taskObject["category"] = task.category;
    // taskObject["isCompleted"] = task.isCompleted;
    // taskObject["date"] = task.date.toString("yyyy-MM-dd");
    // qDebug() << "Task added: " << taskObject;
}

void MainWidget::on_deleteTask_handled(int taskId)
{
        // 根据任务ID删除任务
        for (int i = 0; i < tasks.size(); ++i) {
            if (tasks[i].id == taskId) {
                tasks.removeAt(i);
                break;
            }
        }
    
        // 更新任务列表
        updateTaskListWidget(tasks);
    
        // 保存任务到文件
        saveTasksToFile(tasks);
}
