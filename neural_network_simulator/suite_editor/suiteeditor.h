#ifndef SUITEEDITOR_H
#define SUITEEDITOR_H

#include <QMainWindow>
#include <QList>
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

    QList<ImageData *> suiteList;
    QList<ImageData *>::iterator currentImage;
    unsigned int currentIndex;

    void clearSuiteList();

    bool **allocate_matrix(unsigned int dim);
    void free_mat(bool **m, unsigned int dim);
};

#endif // SUITEEDITOR_H
