/*
 * suiteeditor.cpp
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
#include <QFileDialog>
#include <QMessageBox>
#include <cmath>
#include <fstream>

#include "suiteeditor.h"
#include "ui_suite_editor.h"

SuiteEditor::SuiteEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::suite_editor)
{
    ui->setupUi(this);
    ui->zoomValue->setText(QString::number(ui->numberEditor->zoomFactor()));
    ui->nrInputs->setText(QString::number(ui->numberEditor->getDim()*ui->numberEditor->getDim()));
    ui->nrOutputs->setText("0");
    createActions();
    createMenus();
    currentIndex = 0;
}

SuiteEditor::~SuiteEditor()
{
    delete ui;
    clearSuiteList();
}

void SuiteEditor::clearSuiteList() {
    for ( QList<ImageData *>::iterator iterator = suiteList.begin(); iterator != suiteList.end(); ++iterator) {
        ImageData *image = *iterator;
        free_mat(image->getMatrix(), image->getMatrixDim());
    }
    suiteList.clear();
    ui->numberEditor->clearImage();
}

void SuiteEditor::createActions()
{
    connect(ui->zoomUpdateButton, SIGNAL(clicked(bool)), this, SLOT(updateZoom()));    

    connect(ui->homeButton, SIGNAL(clicked(bool)), this, SLOT(gotoHome()));
    connect(ui->nextButton, SIGNAL(clicked(bool)), this, SLOT(nextImage()));
    connect(ui->previousButton, SIGNAL(clicked(bool)), this, SLOT(previousImage()));

    connect(ui->newButton, SIGNAL(clicked(bool)), this, SLOT(newElement()));
    connect(ui->deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteElement()));
    connect(ui->addButton, SIGNAL(clicked(bool)), this, SLOT(addElement()));

    openSuiteAction = new QAction(tr("&Open Suite"), this);
    openSuiteAction->setShortcut(tr("Ctrl+O"));
    openSuiteAction->setStatusTip(tr("Open a suite file"));
    connect(openSuiteAction, SIGNAL(triggered()), this, SLOT(openSuite()));

    saveAsSuiteAction = new QAction(tr("Save &As Suite"), this);
    saveAsSuiteAction->setShortcut(tr("Ctrl+A"));
    saveAsSuiteAction->setStatusTip(tr("Save as new suite file"));
    connect(saveAsSuiteAction, SIGNAL(triggered()), this, SLOT(saveAsSuite()));

    saveSuiteAction = new QAction(tr("&Save Suite"), this);
    saveSuiteAction->setShortcut(tr("Ctrl+S"));
    saveSuiteAction->setStatusTip(tr("Save to suite file"));
    connect(saveSuiteAction, SIGNAL(triggered()), this, SLOT(saveSuite()));

    newSuiteAction = new QAction(tr("&New Suite"), this);
    newSuiteAction->setShortcut(tr("Ctrl+N"));
    newSuiteAction->setStatusTip(tr("New suite"));
    connect(newSuiteAction, SIGNAL(triggered()), this, SLOT(newSuite()));
}

void SuiteEditor::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openSuiteAction);
    fileMenu->addAction(saveSuiteAction);
    fileMenu->addAction(saveAsSuiteAction);
    fileMenu->addAction(newSuiteAction);
}

void SuiteEditor::updateZoom()
{
    ui->numberEditor->setZoomFactor(ui->zoomValue->text().toInt());
    ui->numberEditor->resize(ui->numberEditor->sizeHint());
    ui->numberEditor->adjustSize();
}

void SuiteEditor::openSuite() {
    std::ifstream file;
    ui->inputFile->clear();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Suite File"), ".", tr("Text files (*.txt)"));
    if ( fileName.isEmpty() )
        return;
    ui->inputFile->insert(fileName);
    clearSuiteList();
    file.open(ui->inputFile->text().toStdString().c_str());
    unsigned int nr;
    file>>nr;
    for ( unsigned int i = 0; i < nr; i++) {
        unsigned int dim1, dim2, dim3;
        file>>dim1>>dim2>>dim3;
        if ( i == 0 ) {
            ui->nrInputs->clear();
            ui->nrInputs->insert(QString::number(dim3));
        }
        ImageData *img = new ImageData();
        dim3 = sqrt(dim3);
        img->setMatrixDim(dim3);
        unsigned int val;
        bool **matrix = allocate_matrix(dim3);
        for ( unsigned int j = 0; j < dim3; j++ ) {
            for ( unsigned int k = 0; k < dim3; k++) {
                file>>val;
                if (val == 1) {
                    matrix[k][j] = true;
                } else {
                    matrix[k][j] = false;
                }
            }
        }
        img->setMatrix(matrix);
        file>>dim1>>dim2>>dim3;
        if ( i == 0) {
            ui->nrOutputs->clear();
            ui->nrOutputs->insert(QString::number(dim3));
        }
        img->setExpectedDim(dim3);
        for ( unsigned int j = 0; j < dim3; j++ ) {
            file>>val;
            if ( val == 1 ) {
                img->setExpectedValue(j);
            }
        }
        suiteList.append(img);
    }
    currentImage = suiteList.begin();
    currentIndex = 0;
    ui->numberEditor->setImage((*currentImage)->getMatrix());
    ui->currentIndex->clear();
    ui->currentIndex->insert(QString::number(currentIndex));
    ui->expectedValue->clear();
    ui->expectedValue->insert(QString::number((*currentImage)->getExpectedValue()));
    file.close();
}

void SuiteEditor::saveSuite() {
    if ( suiteList.isEmpty() )
        return;
    if ( ui->inputFile->text().isEmpty() ) {
        QMessageBox::critical(this, tr("Please do first Save As or provide a fileName"), tr("Please provide a file name to be saved"), QMessageBox::Yes, QMessageBox::No);
        return;
    }
    std::ofstream file;
    file.open(ui->inputFile->text().toStdString().c_str(), std::ios::trunc);
    file<<suiteList.size()<<std::endl;
    for ( QList<ImageData *>::iterator it = suiteList.begin(); it != suiteList.end(); ++it )
        file<<*(*it);
    file.close();
}

void SuiteEditor::saveAsSuite() {
    ui->inputFile->clear();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Suite as File"), ".", tr("Text files (*.txt)"));
    ui->inputFile->insert(fileName);
    saveSuite();
}

void SuiteEditor::newSuite() {
    clearSuiteList();
    currentIndex = -1;
    ui->currentIndex->clear();
    ui->expectedValue->clear();
    ui->numberEditor->clearImage();
}

void SuiteEditor::gotoHome() {
    if ( suiteList.isEmpty() ) {
        ui->numberEditor->clearImage();
        return;
    }
    currentImage = suiteList.begin();
    currentIndex = 0;
    ui->currentIndex->clear();
    ui->currentIndex->insert(QString::number(currentIndex));
    ui->numberEditor->setImage((*currentImage)->getMatrix());
    ui->expectedValue->clear();
    ui->expectedValue->insert(QString::number((*currentImage)->getExpectedValue()));
}

void SuiteEditor::nextImage() {
    if ( suiteList.isEmpty() )
        return;
    currentImage++;
    if ( currentImage != suiteList.end() ) {
        currentIndex++;
        ui->currentIndex->clear();
        ui->currentIndex->insert(QString::number(currentIndex));
        ui->numberEditor->setImage((*currentImage)->getMatrix());
        ui->expectedValue->clear();
        ui->expectedValue->insert(QString::number((*currentImage)->getExpectedValue()));
    } else {
        currentImage = suiteList.end();
        currentImage--;
        ui->currentIndex->clear();
        ui->currentIndex->insert(QString::number(currentIndex));
        ui->expectedValue->clear();
        ui->expectedValue->insert(QString::number((*currentImage)->getExpectedValue()));
    }
}

void SuiteEditor::previousImage() {
    if ( suiteList.isEmpty() )
        return;
    if ( currentImage != suiteList.begin() ) {
        currentImage--;
        currentIndex--;
        ui->currentIndex->clear();
        ui->currentIndex->insert(QString::number(currentIndex));
        ui->numberEditor->setImage((*currentImage)->getMatrix());
        ui->expectedValue->clear();
        ui->expectedValue->insert(QString::number((*currentImage)->getExpectedValue()));
    } else {
        currentImage = suiteList.begin();
        currentIndex = 0;
        ui->currentIndex->clear();
        ui->currentIndex->insert(QString::number(currentIndex));
        ui->expectedValue->clear();
        ui->expectedValue->insert(QString::number((*currentImage)->getExpectedValue()));
    }
}

bool **SuiteEditor::allocate_matrix(unsigned int dim) {
    bool **m = new bool*[dim]();
    for ( unsigned int i = 0; i < dim; i++ ) {
        m[i] = new bool[dim]();
    }
    return m;
}

void SuiteEditor::free_mat(bool **m, unsigned int dim) {
    if ( m == nullptr )
        return;
    for ( unsigned int i = 0; i < dim; i++ ) {
        delete[] m[i];
    }
    delete[] m;
}

void SuiteEditor::newElement() {
    ui->currentIndex->clear();
    ui->expectedValue->clear();
    ui->numberEditor->clearImage();
    currentIndex = suiteList.size();
    ui->currentIndex->insert(QString::number(currentIndex));
    ui->numberEditor->setDim(sqrt(ui->nrInputs->text().toInt()));
}

void SuiteEditor::addElement() {
    ImageData *img = new ImageData();
    img->setMatrixDim(ui->numberEditor->getDim());
    img->setMatrix(ui->numberEditor->getImage());
    img->setExpectedDim(ui->nrOutputs->text().toInt());
    img->setExpectedValue(ui->expectedValue->text().toInt());
    suiteList.append(img);
    currentImage = suiteList.end();
    currentImage--;
}

void SuiteEditor::deleteElement() {
    if ( currentIndex > (suiteList.size() - 1) ) {
        ui->numberEditor->clearImage();
        return;
    }
    suiteList.erase(currentImage);
    free_mat((*currentImage)->getMatrix(), (*currentImage)->getMatrixDim());
    delete *currentImage;
    gotoHome();
}
