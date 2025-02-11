#include "tasklistitem.h"

#include <QHBoxLayout>
#include <QCheckBox>
#include <QPlainTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>


TaskListItem::TaskListItem(const Task &task)
{

}

TaskListItem::TaskListItem(const QString &content, const QString &priority, const QString &category, const QDate &date, bool isCompleted)
    : QListWidgetItem(content),  // 调用 QListWidgetItem 的构造函数，设置任务内容为列表项的文本
      m_content(content),
      m_priority(priority),
      m_category(category),
      m_date(date),
      m_isCompleted(isCompleted)
{
    // 可以在这里根据需求设置任务的显示样式，或者初始化其他属性
    this->setCheckState(isCompleted ? Qt::Checked : Qt::Unchecked);
    QHBoxLayout *layout = new QHBoxLayout();


    QPlainTextEdit *taskContentEdit = new QPlainTextEdit();
    QComboBox *priorityCombo = new QComboBox();
    QComboBox *categoryCombo = new QComboBox();
    QCheckBox *completedCheck = new QCheckBox("Completed");

    priorityCombo->addItems({"High", "Medium", "Low"});
    categoryCombo->addItems({"Dialogue", "Project", "Small task", "Video", "None"});

    layout->addWidget(new QLabel("Task content:"));
    layout->addWidget(taskContentEdit);
    layout->addWidget(new QLabel("Priority:"));
    layout->addWidget(priorityCombo);
    layout->addWidget(new QLabel("Category:"));
    layout->addWidget(categoryCombo);
    layout->addWidget(completedCheck);

    // this->setLayout(layout);



}


QString TaskListItem::getContent() const {
    return m_content;
}

QString TaskListItem::getPriority() const {
    return m_priority;
}

QString TaskListItem::getCategory() const {
    return m_category;
}

QDate TaskListItem::getDate() const {
    return m_date;
}

bool TaskListItem::isTaskCompleted() const {
    return m_isCompleted;
}