#ifndef TASKLISTITEM_H
#define TASKLISTITEM_H

#include <QListWidgetItem>
#include <QString>
#include <QDate>

class Task;

class TaskListItem: public QListWidgetItem
{
public:
    TaskListItem(const Task& task);
    TaskListItem(const QString &content, const QString &priority, const QString &category, const QDate &date, bool isCompleted = false);

    QString getContent() const;
    QString getPriority() const;
    QString getCategory() const;
    QDate getDate() const;
    bool isTaskCompleted() const;

private:
    QString m_content;
    QString m_priority;
    QString m_category;
    QDate m_date;
    bool m_isCompleted;
};

#endif // TASKLISTITEM_H

