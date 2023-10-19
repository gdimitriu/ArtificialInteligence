#include "imagedata.h"

ImageData::ImageData(QObject *parent) : QObject(parent)
{

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
