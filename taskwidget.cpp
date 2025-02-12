#include "taskwidget.h"
#include "ui_taskwidget.h"

# include "common.h"

#include <QDebug>

TaskWidget::TaskWidget(const Task &task,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TaskWidget)
    , taskId(task.id)  // init task id
{
    ui->setupUi(this);

    // 添加优先级和类别
    ui->priorityCombo->addItems(priorityList);
    ui->categoryCombo->addItems(categoryList);

    // 设置任务内容
    ui->plainTextEdit->setPlainText(task.content);
    ui->priorityCombo->setCurrentIndex(task.priority);
    ui->categoryCombo->setCurrentText(task.category);
    // ui->categoryCombo->setCurrentIndex(task.category);
    ui->checkBox->setChecked(task.isCompleted);
    ui->date_label->setText(task.date.toString("yyyy-MM-dd"));

}


TaskWidget::~TaskWidget()
{
    delete ui;
}

void TaskWidget::on_delete_btn_clicked()
{
    qDebug() << "Delete button clicked";

    // 发送删除任务信号,带参数
    emit deleteTask(taskId);
}

