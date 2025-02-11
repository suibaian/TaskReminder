#include "taskwidget.h"
#include "ui_taskwidget.h"

# include "common.h"

TaskWidget::TaskWidget(const Task &task,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TaskWidget)
{
    ui->setupUi(this);

    // 添加优先级和类别
    ui->priorityCombo->addItems(priorityList);
    ui->categoryCombo->addItems(categoryList);

    // 设置任务内容
    ui->plainTextEdit->setPlainText(task.content);
    ui->priorityCombo->setCurrentIndex(task.priority);
    ui->categoryCombo->setCurrentIndex(task.category.toInt());
    ui->checkBox->setChecked(task.isCompleted);
    ui->date_label->setText(task.date.toString("yyyy-MM-dd"));

}


TaskWidget::~TaskWidget()
{
    delete ui;
}
