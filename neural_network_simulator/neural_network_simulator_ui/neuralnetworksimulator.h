#ifndef NEURALNETWORKSIMULATOR_H
#define NEURALNETWORKSIMULATOR_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>


#include "../include/netfeedf.h"

namespace Ui {
class NeuralNetworkSimulator;
}

class NeuralNetworkSimulator : public QMainWindow
{
    Q_OBJECT

public:
    explicit NeuralNetworkSimulator(QWidget *parent = 0);
    ~NeuralNetworkSimulator();

private slots:
    void openTrainingFile();
    void openRunFile();
    void createNetwork();
    void startTrainning();

private:
    Ui::NeuralNetworkSimulator *ui;

    void createActions();
    void createMenu();

    bool checkNetfeedf();
    bool checkNetfeeddl();
    bool checkNetfeedft();
    bool checkTrainningData();

    QAction *openTrainningFileAction;
    QAction *openRunFileAction;
    QMenu *fileMenu;

    net_feedf *network;
};

#endif // NEURALNETWORKSIMULATOR_H
