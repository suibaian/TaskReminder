#include "taskwidget.h"
#include "ui_taskwidget.h"

TaskWidget::TaskWidget(const Task &task,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TaskWidget)
{
    ui->setupUi(this);

    ui->priorityCombo->addItems({"High", "Medium", "Low"});
    ui->categoryCombo->addItems({"Dialogue", "Project", "Small task", "Video", "None"});


    ui->plainTextEdit->setPlainText(task.content);
    ui->priorityCombo->setCurrentIndex(task.priority.toInt());
    ui->categoryCombo->setCurrentIndex(task.category.toInt());

}


TaskWidget::~TaskWidget()
{
    delete ui;
}
