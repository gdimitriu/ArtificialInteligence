#include "suiteeditor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SuiteEditor w;
    w.show();

    return a.exec();
}
