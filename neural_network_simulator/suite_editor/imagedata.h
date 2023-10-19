#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include <QObject>
#include <fstream>
#include <iostream>

class ImageData : public QObject
{
    Q_OBJECT
public:
    explicit ImageData(QObject *parent = nullptr);
    bool **getMatrix() { return matrix; }
    unsigned int getExpectedValue() { return expectedValue; }
    void setMatrix(bool **newMatrix) { matrix = newMatrix; }
    void setExpectedValue(unsigned int newValue) { expectedValue = newValue; }
    void setMatrixDim(unsigned int newDim) { matrixDim = newDim; }
    void setExpectedDim(unsigned int newDim) { expectedDim = newDim; }
    unsigned int getMatrixDim() { return matrixDim; }
    bool *getExpectedVector();
    friend std::ostream& operator <<(std::ostream &out, const ImageData &img);
private:
    bool **matrix;
    unsigned int expectedValue;
    unsigned int matrixDim;
    unsigned int expectedDim;
};

#endif // IMAGEDATA_H
