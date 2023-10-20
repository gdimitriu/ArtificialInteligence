/*
 * imagedata.cpp
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

#include "imagedata.h"

ImageData::ImageData(QObject *parent) : QObject(parent)
{
    matrix = nullptr;
    matrixDim = 0;
    expectedDim = 0;
    expectedValue = 0;
}

bool *ImageData::getExpectedVector() {
   bool *expectedVector = new bool[expectedDim];
   for ( unsigned int i = 0; i < expectedDim; ++i ) {
       if ( i == expectedValue ) {
           expectedVector[i] = true;
       } else {
           expectedVector[i] = false;
       }
   }
   return expectedVector;
}

std::ostream& operator <<(std::ostream &out, const ImageData &img) {
    out<<1<<" "<<1<<" "<<img.matrixDim*img.matrixDim<<std::endl;
    for ( unsigned int j = 0; j < img.matrixDim; j++ ) {
        for ( unsigned int k = 0; k < img.matrixDim; k++) {
            if ( img.matrix[k][j] == true )
                out<<1;
            else
                out<<0;
            if ( k < (img.matrixDim - 1) )
                out<<" ";
        }
        out<<std::endl;
    }
    out<<1<<" "<<1<<" "<<img.expectedDim<<std::endl;
    for ( unsigned int i = 0; i < img.expectedDim; ++i ) {
        if ( i != img.expectedValue )
            out<<0;
        else
            out<<1;
        if ( i < (img.expectedDim - 1) )
            out<<" ";
    }
    out<<std::endl;
    return out;
}
