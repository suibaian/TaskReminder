# ifndef ADDDIALOG_H
# define ADDDIALOG_H

# include <QDialog>
# include <QDate>


namespace Ui {
class AddDialog;
}

class Task {
    public:
        QString content;  // 任务内容
        QString priority;  // 任务优先级
        QString category;  // 任务类别
        bool isCompleted;  // 是否完成
        QDate date;  // 创建日期
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


};



# endif // ADDDIALOG_H

