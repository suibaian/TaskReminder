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
#include <QKeyEvent>

#include "taskwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);


        // set app icon and title 
        this->setWindowIcon(QIcon("://res/TaskReminder.ico"));
        this->setWindowTitle("Task Manager");


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

    // 启用平滑滚动
    ui->task_listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    // 调整滚动参数
    ui->task_listWidget->verticalScrollBar()->setSingleStep(20); //20一次滚动的距离，自行调整




    // 连接信号和槽

    // 连接 addTask_pushButton 的 clicked 信号和 on_addTask_pushButton_clicked 槽函数
    connect(ui->addTask_pushButton, &QPushButton::clicked, this, &MainWidget::on_createNewTask_Dialog);
    // connect filter_comboBox for filter
    connect(ui->class_comboBox, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), [=](const QString &text) {
        // qDebug() << "Filter by category: " << text;
        QList<Task> filteredTasks = filterTasks(tasks, text);
        updateTaskListWidget(filteredTasks);
    });

    // connect sort_comboBox for sort
    connect(ui->sort_comboBox, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), [=](const QString &text) {
        // qDebug() << "Sort by: " << text;
        sortTasks(tasks, text);
        updateTaskListWidget(tasks);
    });

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
        taskObject["date"] = task.date.toString(TASK_DATE_FORMAT);
        taskArray.append(taskObject);
    }

    QJsonDocument doc(taskArray);
    // to backup one file about previous tasks

    QFile backupfile(TASK_BACKUP_FILE_NAME);
    if (backupfile.open(QIODevice::WriteOnly)) {  // 使用 WriteOnly 模式覆盖文件内容
        backupfile.write(doc.toJson());
        backupfile.close();
        qDebug() << "Tasks saved to file!";
    } else {
        qDebug() << "Failed to open file for writing!";
    }

    backupfile.close();

    // to save tasks to file
    QFile file(TASK_FILE_NAME);

    if (file.open(QIODevice::WriteOnly)) {  // 使用 WriteOnly 模式覆盖文件内容
        file.write(doc.toJson());
        file.close();
        qDebug() << "Tasks saved to file!";
    } else {
        qDebug() << "Failed to open file for writing!";
    }
}

bool MainWidget::loadTasksFromFile() {
    QFile file(TASK_FILE_NAME);
    
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


// only update task data not update task list widget
void MainWidget::updateTaskItem(const Task &task)
{
    for (int i = 0; i < tasks.size(); ++i) {
        if (tasks[i].id == task.id) {
            tasks[i] = task;
            break;
        }
    }

    // 保存任务到文件
    saveTasksToFile(tasks);
}

// only update task list widget not save to file
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
    // create a new item
    QListWidgetItem *item = new QListWidgetItem();
    ui->task_listWidget->addItem(item);

    // create a new myself want TaskWidget
    TaskWidget *taskWidget = new TaskWidget(task);
    ui->task_listWidget->setItemWidget(item, taskWidget);  // set item widget
    item->setSizeHint(QSize(800, 150));
    ui->task_listWidget->setCurrentItem(item);

    // set background color
    if (ui->task_listWidget->count() % 2 == 0) {
        item->setBackground(QColor(255, 255, 255));  // 白色
    } else {
        item->setBackground(QColor(240, 240, 240));  // 浅灰色
    }

    // 连接 TaskWidget 的 deleteTask 信号 到 MainWidget 的 deleteTask 槽函数
    connect(taskWidget, &TaskWidget::deleteTask, this, &MainWidget::on_deleteTask_handled);
    // 连接 TaskWidget 的 taskChanged 信号 到 MainWidget 的 updateTaskItem 槽函数
    connect(taskWidget, &TaskWidget::taskChanged, this, &MainWidget::updateTaskItem);
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
    // TODO:add task to tasks list but add first task to the top of the list
    // tasks.prepend(task);
    tasks.append(task);

    // // 更新任务列表
    // updateTaskListWidget(tasks);
    addTask(task);

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

void MainWidget::keyPressEvent(QKeyEvent *event)
{
    // if ctrl + s is save 
    if (event->key() == Qt::Key_S && (event->modifiers() & Qt::ControlModifier))
    {
        saveTasksToFile(tasks);
    }
}
