/*
 * neuralnetworksimulator.cpp
 *
 *  @Copyright 2023 Gabriel Dimitriu
 * All rights reserved.
 *
 * This file is part of Neural Network Simulator project.

 * Neural Network Simulator is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * Neural Network Simulator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with Neural Network Simulator; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

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
    int maxVector(matrixf *outputs);
    bool **allocate_matrix(unsigned int dim);
    void free_mat(bool **m, unsigned int dim);

    net_feedf *network;
    enum FILE_TYPE {
        TRAINING_SUITE,
        TEST_SUITE,
        RUN_FILE,
        NETWORK
    } fileType;
};

#endif // NEURALNETWORKSIMULATOR_H
