#ifndef SUITE_EDITOR_H
#define SUITE_EDITOR_H

#include <QMainWindow>

namespace Ui {
class suite_editor;
}

class suite_editor : public QMainWindow
{
    Q_OBJECT

public:
    explicit suite_editor(QWidget *parent = 0);
    ~suite_editor();

private:
    Ui::suite_editor *ui;
};

#endif // SUITE_EDITOR_H
