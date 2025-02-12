#include "taskwidget.h"
#include "ui_taskwidget.h"

# include "common.h"

#include <QDebug>

TaskWidget::TaskWidget(const Task &task,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TaskWidget)
{
    ui->setupUi(this);


    _task = new Task(task);


    // 添加优先级和类别
    ui->priorityCombo->addItems(priorityList);
    ui->categoryCombo->addItems(categoryList);

    // 设置任务内容
    ui->plainTextEdit->setPlainText(task.content);
    ui->priorityCombo->setCurrentIndex(task.priority);
    ui->categoryCombo->setCurrentText(task.category);
    // ui->categoryCombo->setCurrentIndex(task.category);
    ui->checkBox->setChecked(task.isCompleted);
    ui->date_label->setText(task.date.toString(TASK_DATE_FORMAT));


    // 连接当发生更改时，发送信号
    connect(ui->plainTextEdit, &QPlainTextEdit::textChanged, [=]() {
        _task->content = ui->plainTextEdit->toPlainText();
        emit taskChanged(*_task);
    });

    connect(ui->priorityCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
        _task->priority = index;
        emit taskChanged(*_task);
    });

    connect(ui->categoryCombo, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), [=](const QString &text) {
        _task->category = text;
        emit taskChanged(*_task);
    });

    connect(ui->checkBox, &QCheckBox::stateChanged, [=](int state) {
        _task->isCompleted = state == Qt::Checked;
        emit taskChanged(*_task);
    });

}


TaskWidget::~TaskWidget()
{
    delete ui;
}

// there is not deleteTask but only emit deleteTask singal
void TaskWidget::on_delete_btn_clicked()
{
    qDebug() << "Delete button clicked";

    // 发送删除任务信号,带参数
    emit deleteTask(_task->id);
}

