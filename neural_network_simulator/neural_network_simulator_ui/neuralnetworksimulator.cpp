#include "neuralnetworksimulator.h"
#include "ui_neuralnetworksimulator.h"

#include <QFileDialog>
#include <QMessageBox>

#include "../include/netfeedf.h"
#include "../include/netfeedl.h"
#include "../include/netfeedft.h"
#include "../include/netfeedftp.h"

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
}

NeuralNetworkSimulator::~NeuralNetworkSimulator()
{
    delete ui;
    delete network;
}


void NeuralNetworkSimulator::createActions() {
    openTrainningFileAction = new QAction(tr("Open &Trainning"), this);
    openTrainningFileAction->setShortcut(tr("Ctrl+T"));
    openTrainningFileAction->setStatusTip(tr("Open trainning file"));
    connect(openTrainningFileAction, SIGNAL(triggered()), this, SLOT(openTrainingFile()));

    openRunFileAction = new QAction(tr("Open &Running"), this);
    openRunFileAction->setShortcut(tr("Ctrl+R"));
    openRunFileAction->setStatusTip(tr("Open Running file"));
    connect(openRunFileAction, SIGNAL(triggered()), this, SLOT(openRunFile()));

    connect(ui->createButton, SIGNAL(clicked()), this, SLOT(createNetwork()));
    connect(ui->trainningButton, SIGNAL(clicked()), this, SLOT(startTrainning()));
}

void NeuralNetworkSimulator::createMenu() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openTrainningFileAction);
    fileMenu->addAction(openRunFileAction);
}

void NeuralNetworkSimulator::openTrainingFile() {
    ui->inputFile->clear();
    ui->inputFile->insert(QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Text files (*.txt)")));
}

void NeuralNetworkSimulator::openRunFile() {
    ui->inputFile->clear();
    ui->inputFile->insert(QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Text files (*.txt)")));
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
    network->create();
    network->init_net();
    network->set_nr_it(ui->nrOfIterations->text().toInt());
    network->set_temp(1);
    network->setEta(ui->eta->text().toFloat());
    network->setMomentum(ui->momentum->text().toFloat());
    network->setDesiredMse(ui->mseError->text().toFloat());
    network->load_trainning_suite_text_file(const_cast<char *>(ui->inputFile->text().toStdString().c_str()));
    network->train();

}
