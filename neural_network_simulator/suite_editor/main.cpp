#include "suite_editor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    suite_editor w;
    w.show();

    return a.exec();
}
