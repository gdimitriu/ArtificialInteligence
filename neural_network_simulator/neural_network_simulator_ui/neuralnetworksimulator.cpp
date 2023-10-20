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

#include "neuralnetworksimulator.h"
#include "ui_neuralnetworksimulator.h"

#include <QFileDialog>
#include <QMessageBox>

#include "../include/netfeedf.h"
#include "../include/netfeedl.h"
#include "../include/netfeedft.h"
#include "../include/netfeedftp.h"

static int max(matrixf &out) {
    int index = 0;
    float maxim = out(0, 0, 0);
    for (int i = 1; i < out.d2(); i++) {
        if (maxim < out(0, 0, i)) {
            maxim = out(0, 0, i);
            index = i;
        }
    }
    return index;
}

NeuralNetworkSimulator::NeuralNetworkSimulator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NeuralNetworkSimulator)
{
    ui->setupUi(this);
    createActions();
    createMenu();
    ui->networkType->addItem("netfeedl");
    ui->networkType->addItem("netfeedf");
    ui->networkType->addItem("netfeedft");
    ui->networkType->addItem("netfeedftp");

    network = nullptr;
    initDefaults();
}

NeuralNetworkSimulator::~NeuralNetworkSimulator()
{
    delete ui;
    delete network;
}


void NeuralNetworkSimulator::createActions() {
    openTrainningSuiteFileAction = new QAction(tr("Open &Trainning Suite"), this);
    openTrainningSuiteFileAction->setShortcut(tr("Ctrl+T"));
    openTrainningSuiteFileAction->setStatusTip(tr("Open trainning file suite"));
    connect(openTrainningSuiteFileAction, SIGNAL(triggered()), this, SLOT(openTrainingSuiteFile()));

    openTesSuitetFileAction = new QAction(tr("Open &test Suite"), this);
    openTesSuitetFileAction->setShortcut(tr("Ctrl+t"));
    openTesSuitetFileAction->setStatusTip(tr("Open test suite file"));
    connect(openTesSuitetFileAction, SIGNAL(triggered()), this, SLOT(openTestSuiteFile()));

    openRunFileAction = new QAction(tr("Open &Running"), this);
    openRunFileAction->setShortcut(tr("Ctrl+R"));
    openRunFileAction->setStatusTip(tr("Open Running file"));
    connect(openRunFileAction, SIGNAL(triggered()), this, SLOT(openRunFile()));

    openNetworkFileAction = new QAction(tr("&Open Network"), this);
    openNetworkFileAction->setShortcut(tr("Ctrl+O"));
    openNetworkFileAction->setStatusTip(tr("Open Network file"));
    connect(openNetworkFileAction, SIGNAL(triggered()), this, SLOT(openNetworkFile()));

    saveNetworkFileAction = new QAction(tr("&Save Network"), this);
    saveNetworkFileAction->setShortcut(tr("Ctrl+S"));
    saveNetworkFileAction->setStatusTip(tr("Save Network file"));
    connect(saveNetworkFileAction, SIGNAL(triggered()), this, SLOT(saveNetworkFile()));

    connect(ui->createButton, SIGNAL(clicked()), this, SLOT(createNetwork()));
    connect(ui->trainningButton, SIGNAL(clicked()), this, SLOT(startTrainning()));
    connect(ui->runButton, SIGNAL(clicked()), this, SLOT(startRunning()));
}

void NeuralNetworkSimulator::createMenu() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openTrainningSuiteFileAction);
    fileMenu->addAction(openTesSuitetFileAction);
    fileMenu->addAction(openRunFileAction);

    networkMenu = menuBar()->addMenu(tr("&Network"));
    networkMenu->addAction(openNetworkFileAction);
    networkMenu->addAction(saveNetworkFileAction);
}

void NeuralNetworkSimulator::openTrainingSuiteFile() {
    ui->inputFile->clear();
    ui->inputFile->insert(QFileDialog::getOpenFileName(this, tr("Open Trainning Suite File"), ".", tr("Text files (*.txt)")));
    fileType = TRAINING_SUITE;
}

void NeuralNetworkSimulator::openTestSuiteFile() {
    ui->inputFile->clear();
    ui->inputFile->insert(QFileDialog::getOpenFileName(this, tr("Open Test Suite File"), ".", tr("Text files (*.txt)")));
    fileType = TEST_SUITE;
}

void NeuralNetworkSimulator::openRunFile() {
    ui->inputFile->clear();
    ui->inputFile->insert(QFileDialog::getOpenFileName(this, tr("Open Run File"), ".", tr("Text files (*.txt)")));
    fileType = RUN_FILE;
}

void NeuralNetworkSimulator::openNetworkFile() {
    ui->inputFile->clear();
    ui->inputFile->insert(QFileDialog::getOpenFileName(this, tr("Open Network File"), ".", tr("Dat files (*.dat)")));
    fileType = NETWORK;
}

void NeuralNetworkSimulator::saveNetworkFile() {
    ui->inputFile->clear();
    ui->inputFile->insert(QFileDialog::getSaveFileName(this, tr("Save Network File"), ".", tr("Dat files (*.dat)")));
    fileType = NETWORK;
    if ( network == nullptr ) {
        QMessageBox::critical(this, tr("First create the network and train it."), tr("First create the network and train it."), QMessageBox::Yes, QMessageBox::No);
        return;
    }
    if ( ON != network->trained() ) {
        QMessageBox::critical(this, tr("First train the network."), tr("First train the network."), QMessageBox::Yes, QMessageBox::No);
        return;
    }
    network->save(const_cast<char *>(ui->inputFile->text().toStdString().c_str()));
}

void NeuralNetworkSimulator::initDefaults() {
    ui->nrOfHiddenLevels->setText("0");
    ui->nrOfInputs->setText("0");
    ui->nrOfHidden1->setText("0");
    ui->nrOfHidden2->setText("0");
    ui->nrOfOutputs->setText("0");
    ui->dimInputRectangle->setText("0");
    ui->distInputRect->setText("0");
    ui->dimConnectionRect->setText("0");
    ui->distConnRect->setText("0");
    ui->nrOfMapsFirstLevel->setText("0");
    ui->nrOfMapsSecondLevel->setText("0");
}

bool NeuralNetworkSimulator::checkNetfeedf() {
    bool ok;
    ui->nrOfHiddenLevels->text().toInt(&ok);
    if ( !ok ) {
        QMessageBox::critical(this, tr("Nr of Hidden Levels is not a number"), "current value is " +  ui->nrOfHiddenLevels->text(), QMessageBox::Yes, QMessageBox::No);
        return false;
    }
    ui->nrOfInputs->text().toInt(&ok);
    if ( !ok ) {
        QMessageBox::critical(this, tr("Nr of Inputs is not a number"), "current value is " +  ui->nrOfInputs->text(), QMessageBox::Yes, QMessageBox::No);
        return false;
    }
    ui->nrOfHidden1->text().toInt(&ok);
    if ( !ok ) {
        QMessageBox::critical(this, tr("Nr of Hidden 1 level is not a number"), "current value is " +  ui->nrOfHidden1->text(), QMessageBox::Yes, QMessageBox::No);
        return false;
    }
    ui->nrOfHidden2->text().toInt(&ok);
    if ( !ok ) {
        QMessageBox::critical(this, tr("Nr of Hidden 2 is not a number"), "current value is " +  ui->nrOfHidden2->text(), QMessageBox::Yes, QMessageBox::No);
        return false;
    }
    ui->nrOfOutputs->text().toInt(&ok);
    if ( !ok ) {
        QMessageBox::critical(this, tr("Nr of Outputs is not a number"), "current value is " +  ui->nrOfOutputs->text(), QMessageBox::Yes, QMessageBox::No);
        return false;
    }
    return true;
}

bool NeuralNetworkSimulator::checkNetfeeddl() {
    bool ok;
    if ( !checkNetfeedf() ) {
        return false;
    }
    ui->dimInputRectangle->text().toInt(&ok);
    if ( !ok ) {
        QMessageBox::critical(this, tr("Dim of Input Rect is not a number"), "current value is " +  ui->dimInputRectangle->text(), QMessageBox::Yes, QMessageBox::No);
        return false;
    }
    ui->distInputRect->text().toInt(&ok);
    if ( !ok ) {
        QMessageBox::critical(this, tr("Distance of Input Rect is not a number"), "current value is " +  ui->distInputRect->text(), QMessageBox::Yes, QMessageBox::No);
        return false;
    }
    ui->dimConnectionRect->text().toInt(&ok);
    if ( !ok ) {
        QMessageBox::critical(this, tr("Dim of Connection Rect is not a number"), "current value is " +  ui->dimConnectionRect->text(), QMessageBox::Yes, QMessageBox::No);
        return false;
    }
    ui->distConnRect->text().toInt(&ok);
    if ( !ok ) {
        QMessageBox::critical(this, tr("Distance of Connection Rect is not a number"), "current value is " +  ui->distConnRect->text(), QMessageBox::Yes, QMessageBox::No);
        return false;
    }
    return true;
}

bool NeuralNetworkSimulator::checkNetfeedft() {
    bool ok;
    if ( !checkNetfeeddl() ) {
        return false;
    }
    ui->nrOfMapsFirstLevel->text().toInt(&ok);
    if ( !ok ) {
        QMessageBox::critical(this, tr("Nr of Maps first level is not a number"), "current value is " +  ui->nrOfMapsFirstLevel->text(), QMessageBox::Yes, QMessageBox::No);
        return false;
    }
    ui->nrOfMapsSecondLevel->text().toInt(&ok);
    if ( !ok ) {
        QMessageBox::critical(this, tr("Nr of Maps second level is not a number"), "current value is " +  ui->nrOfMapsSecondLevel->text(), QMessageBox::Yes, QMessageBox::No);
        return false;
    }
    return true;
}

void NeuralNetworkSimulator::createNetwork() {
    if ( ui->networkType->currentText() == "netfeedf" ) {
        if ( !checkNetfeedf() )  {
            return;
        }
        if ( network != nullptr ) {
            delete network;
        }
        network = new net_feedf(ui->nrOfHiddenLevels->text().toInt(), ui->nrOfInputs->text().toInt(), ui->nrOfHidden1->text().toInt(), ui->nrOfHidden2->text().toInt(),
                                ui->nrOfOutputs->text().toInt());
        QMessageBox::information(this, ui->networkType->currentText(), "Network " + ui->networkType->currentText() + " created !!", QMessageBox::Yes);
    } else if ( ui->networkType->currentText() == "netfeedl" ) {
        if ( !checkNetfeeddl() ) {
            return;
        }
        if ( network != nullptr ) {
            delete network;
        }
        network = new net_feedl(ui->nrOfHiddenLevels->text().toInt(), ui->nrOfInputs->text().toInt(), ui->nrOfHidden1->text().toInt(), ui->nrOfHidden2->text().toInt(),
                                ui->nrOfOutputs->text().toInt(), ui->dimInputRectangle->text().toInt(), ui->distInputRect->text().toInt(), ui->dimConnectionRect->text().toInt(),
                                ui->distConnRect->text().toInt());
        QMessageBox::information(this, ui->networkType->currentText(), "Network " + ui->networkType->currentText() + " created !!", QMessageBox::Yes);
    } else if ( ui->networkType->currentText() == "netfeedft" ) {
        if ( !checkNetfeedft() ) {
            return;
        }
        if ( network != nullptr ) {
            delete network;
        }
        network = new net_feedft(ui->nrOfHiddenLevels->text().toInt(), ui->nrOfInputs->text().toInt(), ui->nrOfHidden1->text().toInt(), ui->nrOfHidden2->text().toInt(),
                                 ui->nrOfOutputs->text().toInt(), ui->dimInputRectangle->text().toInt(), ui->distInputRect->text().toInt(), ui->dimConnectionRect->text().toInt(),
                                 ui->distConnRect->text().toInt(), ui->nrOfMapsFirstLevel->text().toInt(), ui->nrOfMapsSecondLevel->text().toInt());
        QMessageBox::information(this, ui->networkType->currentText(), "Network " + ui->networkType->currentText() + " created !!", QMessageBox::Yes);
    } else if ( ui->networkType->currentText() == "netfeedftp") {
        if ( !checkNetfeedft() ) {
            return;
        }
        if ( network != nullptr ) {
            delete network;
        }
        network = new net_feedftp(ui->nrOfHiddenLevels->text().toInt(), ui->nrOfInputs->text().toInt(), ui->nrOfHidden1->text().toInt(), ui->nrOfHidden2->text().toInt(),
                                  ui->nrOfOutputs->text().toInt(), ui->dimInputRectangle->text().toInt(), ui->distInputRect->text().toInt(), ui->dimConnectionRect->text().toInt(),
                                  ui->distConnRect->text().toInt(), ui->nrOfMapsFirstLevel->text().toInt(), ui->nrOfMapsSecondLevel->text().toInt());
        QMessageBox::information(this, ui->networkType->currentText(), "Network " + ui->networkType->currentText() + " created !!", QMessageBox::Yes);
    }
}

bool NeuralNetworkSimulator::checkTrainningData() {
    bool ok;
    ui->nrOfIterations->text().toInt(&ok);
    if ( !ok ) {
        QMessageBox::critical(this, tr("Nr of iterations is not a number"), "current value is " +  ui->nrOfIterations->text(), QMessageBox::Yes, QMessageBox::No);
        return false;
    }
    ui->eta->text().toFloat(&ok);
    if ( !ok ) {
        QMessageBox::critical(this, tr("Eta is not a float number"), "current value is " +  ui->eta->text(), QMessageBox::Yes, QMessageBox::No);
        return false;
    }
    ui->mseError->text().toFloat(&ok);
    if ( !ok ) {
        QMessageBox::critical(this, tr("Desired MSE error is not a float number"), "current value is " +  ui->mseError->text(), QMessageBox::Yes, QMessageBox::No);
        return false;
    }
    ui->momentum->text().toFloat(&ok);
    if ( !ok ) {
        QMessageBox::critical(this, tr("Momentum is not a float number"), "current value is " +  ui->momentum->text(), QMessageBox::Yes, QMessageBox::No);
        return false;
    }
    ui->activationTemp->text().toFloat(&ok);
    if ( !ok ) {
        QMessageBox::critical(this, tr("Neuron Activation is not a float number"), "current value is " +  ui->activationTemp->text(), QMessageBox::Yes, QMessageBox::No);
        return false;
    }
    return true;
}

void NeuralNetworkSimulator::startTrainning() {
    if ( !checkTrainningData() ) {
        return;
    }
    if ( network == nullptr ) {
        QMessageBox::information(this, ui->networkType->currentText(), tr("First create the network"), QMessageBox::Yes);
        return;
    }
    network->free_net();
    network->create();
    network->init_net();
    network->set_nr_it(ui->nrOfIterations->text().toInt());
    network->set_temp(ui->activationTemp->text().toFloat());
    network->setEta(ui->eta->text().toFloat());
    network->setMomentum(ui->momentum->text().toFloat());
    network->setDesiredMse(ui->mseError->text().toFloat());
    network->initweights(-0.5, 0.2);
    network->load_trainning_suite_text_file(const_cast<char *>(ui->inputFile->text().toStdString().c_str()));
    network->train();
    if ( ON == network->trained() ) {
        ui->outputDisplay->clear();
        ui->outputDisplay->appendPlainText("Network trainned!");
    } else {
        ui->outputDisplay->clear();
        ui->outputDisplay->appendPlainText("Network failed to be trainned!");
    }
    network->del_trainning_suite();
}

void NeuralNetworkSimulator::startRunning() {
    if ( network == nullptr ) {
        QMessageBox::critical(this, tr("First create the network and train it."), tr("First create the network and train it."), QMessageBox::Yes, QMessageBox::No);
        return;
    }
    if ( ON != network->trained() ) {
        QMessageBox::critical(this, tr("First train the network."), tr("First train the network."), QMessageBox::Yes, QMessageBox::No);
        return;
    }
    ui->outputDisplay->clear();
    if ( TEST_SUITE == fileType ) {
        network->load_test_suite_text_file(const_cast<char *>(ui->inputFile->text().toStdString().c_str()));
        for ( int i = 0 ;i < network->get_nr_in_test_suite(); i++ ) {
            matrixf &inputs = network->get_test_inp(i);
            matrixf &expected = network->get_test_tp(i);
            network->init_inp(inputs);
            network->forward();
            matrixf &outputs = network->get_outputs();
            ui->outputDisplay->appendPlainText("\nTest vector number: " + QString::number(i));
            ui->outputDisplay->appendPlainText("Output vector:");
            for (int i = 0; i < outputs.d0(); i++) {
                for (int j = 0; j < outputs.d1(); j++) {
                    QString str;
                    for (int k = 0; k < outputs.d2(); k++) {
                        str.append(QString::number(outputs(i,j,k)) + " ");
                    }
                    ui->outputDisplay->appendPlainText(str);
                }
            }
            ui->outputDisplay->appendPlainText("Expected vector:");
            for (int i = 0; i < expected.d0(); i++) {
                for (int j = 0; j < expected.d1(); j++) {
                    QString str;
                    for (int k = 0; k < expected.d2(); k++ )
                        str.append(QString::number(expected(i,j,k)) + " ");
                    ui->outputDisplay->appendPlainText(str);
                }
            }
        }
        network->del_test_suite();
    }
}
