/*
 * imagedata.h
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
