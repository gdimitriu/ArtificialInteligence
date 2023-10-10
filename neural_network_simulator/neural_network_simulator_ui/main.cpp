#include "neuralnetworksimulator.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NeuralNetworkSimulator w;
    w.show();

    return a.exec();
}
