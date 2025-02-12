#ifndef COMMON_H
#define COMMON_H

#include <QStringList>

const QStringList priorityList = {"1", "2", "3"};
const QStringList categoryList = {"None","Dialogue", "Project", "Small task", "Video", };

const QString TASK_FILE_NAME = "tasks.json";
const QString TASK_BACKUP_FILE_NAME = "tasks_backup.json";
// const QString TASK_DATE_FORMAT = "yyyy-MM-dd-HH-mm-ss";
const QString TASK_DATE_FORMAT = "yyyy-MM-dd";


#endif // COMMON_H