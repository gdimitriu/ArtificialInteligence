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
    void openTrainingSuiteFile();
    void openRunFile();
    void openTestSuiteFile();
    void createNetwork();
    void startTrainning();
    void startRunning();
    void openNetworkFile();
    void saveNetworkFile();

private:
    Ui::NeuralNetworkSimulator *ui;

    void createActions();
    void createMenu();

    bool checkNetfeedf();
    bool checkNetfeeddl();
    bool checkNetfeedft();
    bool checkTrainningData();

    QAction *openTrainningSuiteFileAction;
    QAction *openTesSuitetFileAction;
    QAction *openRunFileAction;
    QMenu *fileMenu;

    QAction *openNetworkFileAction;
    QAction *saveNetworkFileAction;
    QMenu *networkMenu;

    void initDefaults();

    net_feedf *network;
    enum FILE_TYPE {
        TRAINING_SUITE,
        TEST_SUITE,
        RUN_FILE,
        NETWORK
    } fileType;
};

#endif // NEURALNETWORKSIMULATOR_H
