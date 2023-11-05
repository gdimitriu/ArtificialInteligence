/*
 * suiteeditor.h
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

#ifndef SUITEEDITOR_H
#define SUITEEDITOR_H

#include <QMainWindow>
#include <QLinkedList>
#include <QMenu>
#include <QAction>

#include "imagedata.h"

namespace Ui {
class suite_editor;
}

class SuiteEditor : public QMainWindow
{
    Q_OBJECT
private slots:
    void updateZoom();
    void openSuite();
    void saveSuite();
    void saveAsSuite();
    void newSuite();
    void gotoHome();
    void nextImage();
    void previousImage();
    void newElement();
    void deleteElement();
    void addElement();
public:
    explicit SuiteEditor(QWidget *parent = 0);    
    ~SuiteEditor();


private:
    Ui::suite_editor *ui;

    void createActions();
    void createMenus();

    QMenu *fileMenu;
    QAction *openSuiteAction;
    QAction *saveSuiteAction;
    QAction *saveAsSuiteAction;
    QAction *newSuiteAction;

    QLinkedList<ImageData *> suiteList;
    QLinkedList<ImageData *>::iterator currentImage;
    int currentIndex;

    void clearSuiteList();

    bool **allocate_matrix(unsigned int dim);
    void free_mat(bool **m, unsigned int dim);
};

#endif // SUITEEDITOR_H
