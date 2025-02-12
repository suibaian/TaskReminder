#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QWidget>
#include "adddialog.h"

namespace Ui {
class TaskWidget;
}

class TaskWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TaskWidget(const Task& task, QWidget *parent = nullptr);
    ~TaskWidget();

signals:
    void deleteTask(int taskId);
    void taskChanged(const Task& task);

private slots:
    void on_delete_btn_clicked();

private:
    Ui::TaskWidget *ui;
    Task* _task;  // add task id to widget for find to delete
    // int taskId;  
};

#endif // TASKWIDGET_H
