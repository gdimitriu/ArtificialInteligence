#-------------------------------------------------
#
# Project created by QtCreator 2023-10-08T23:27:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = neural_network_simulator_ui
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        neuralnetworksimulator.cpp

HEADERS += \
        neuralnetworksimulator.h \
    ../include/matrix.h \
    ../include/matrixf.h \
    ../include/netfeedf.h \
    ../include/netfeedft.h \
    ../include/netfeedftp.h \
    ../include/netfeedl.h

FORMS += \
        neuralnetworksimulator.ui

unix:!macx: LIBS += -L$$PWD/../lib/ -lneuralnetwork

INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include
