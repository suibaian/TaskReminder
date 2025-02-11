#include "taskwidget.h"
#include "ui_taskwidget.h"

TaskWidget::TaskWidget(const Task &task,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TaskWidget)
{
    ui->setupUi(this);
}


TaskWidget::~TaskWidget()
{
    delete ui;
}
