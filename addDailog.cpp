#include "addDailog.h"

#include <QVBoxLayout>
#include <QLabel>
// #include <QLineEdit>
// #include <QTextEdit>
#include <QPlainTextEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>

AddDialog::AddDialog(QWidget *parent)
{
    
    QVBoxLayout *layout = new QVBoxLayout(this);

    QPlainTextEdit *taskContentEdit = new QPlainTextEdit(this);
    QComboBox *priorityCombo = new QComboBox(this);
    QComboBox *categoryCombo = new QComboBox(this);
    QCheckBox *completedCheck = new QCheckBox("Completed", this);

    priorityCombo->addItems({"1", "2", "3"});
    categoryCombo->addItems({"Dialogue", "Project", "Small task", "Video", "None"});

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
        Task newTask;
        newTask.content = taskContentEdit->toPlainText();
        newTask.category = categoryCombo->currentText().toInt();
        newTask.isCompleted = completedCheck->isChecked();
        newTask.date = QDate::currentDate();
        emit taskAdded(newTask);
        this->close();
    });

    QObject::connect(cancelButton, &QPushButton::clicked, [=]()
                     {this->close(); });

    // connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::reject);

}

AddDialog::~AddDialog()
{
}