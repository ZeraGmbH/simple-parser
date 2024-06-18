#ifndef TEST_TASK_OPEN_H
#define TEST_TASK_OPEN_H

#include <QObject>

class test_task_open : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void openFailNoServerLocal();
    void openFailNoServerForeign();
    void openSuccess();
};

#endif // TEST_TASK_OPEN_H
