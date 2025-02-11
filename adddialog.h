#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
# include <QDate>

namespace Ui {
class AddDialog;
}


class Task {
    public:
        QString content;  // 任务内容
        int priority;  // 任务优先级
        QString category;  // 任务类别
        bool isCompleted;  // 是否完成
        QDate date;  // 创建日期
    
    
        public:
        Task() : isCompleted(false), priority(0) {}
    
        // 构造函数，用于初始化Task对象
        Task(const QString& cat, const QString& cont, const QDate& d, bool completed, int prio)
            : category(cat), content(cont), date(d), isCompleted(completed), priority(prio) {}
    };

class AddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDialog(QWidget *parent = nullptr);
    ~AddDialog();

    
    signals:
    void taskAdded(const Task &task);

private:
    Ui::AddDialog *ui;
};

#endif // ADDDIALOG_H
