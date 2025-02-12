#include "adddialog.h"
#include "ui_adddialog.h"

#include "common.h"

#include <QVBoxLayout>
#include <QLabel>
// #include <QLineEdit>
// #include <QTextEdit>
#include <QPlainTextEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>

// 初始化静态变量
int Task::nextId = 1;

AddDialog::AddDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddDialog)
{
    ui->setupUi(this);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QPlainTextEdit *taskContentEdit = new QPlainTextEdit(this);
    QComboBox *priorityCombo = new QComboBox(this);
    QComboBox *categoryCombo = new QComboBox(this);
    QCheckBox *completedCheck = new QCheckBox("Completed", this);

    priorityCombo->addItems(priorityList);
    categoryCombo->addItems(categoryList);

    QPushButton *okButton = new QPushButton("OK", this);
    QPushButton *cancelButton = new QPushButton("Cancel", this);

    layout->addWidget(new QLabel("Task content:"));
    layout->addWidget(taskContentEdit);
    layout->addWidget(new QLabel("Priority:"));
    layout->addWidget(priorityCombo);
    layout->addWidget(new QLabel("Category:"));
    layout->addWidget(categoryCombo);
    layout->addWidget(completedCheck);
    layout->addWidget(okButton);
    layout->addWidget(cancelButton);

    this->setLayout(layout);

    QObject::connect(okButton, &QPushButton::clicked, [=]()
                     {
        // Task newTask;
        // newTask.content = taskContentEdit->toPlainText();
        // newTask.category = categoryCombo->currentText().toInt();
        // newTask.isCompleted = completedCheck->isChecked();
        // newTask.date = QDate::currentDate();
        // emit taskAdded(newTask);
        // this->close();

        const QString &cat = categoryCombo->currentText();
        const QString &cont = taskContentEdit->toPlainText();
        const QDate &date = QDate::currentDate();
        bool completed = completedCheck->isChecked();
        int prio = priorityCombo->currentIndex();

        Task newTask = Task(cat, cont, date,completed, prio);
        emit taskAdded(newTask);
    
    });

    QObject::connect(cancelButton, &QPushButton::clicked, [=]()
                     {this->close(); });

    // connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::reject);

}

AddDialog::~AddDialog()
{
    delete ui;
}
