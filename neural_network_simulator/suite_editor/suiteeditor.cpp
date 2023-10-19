#include "suite_editor.h"
#include "ui_suite_editor.h"

suite_editor::suite_editor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::suite_editor)
{
    ui->setupUi(this);
}

suite_editor::~suite_editor()
{
    delete ui;
}
