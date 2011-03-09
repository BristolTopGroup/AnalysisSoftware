/*
 * VariableReader.cpp
 *
 *  Created on: 24 Aug 2010
 *      Author: kreczko
 */

#include "../../interface/Readers/VariableReader.h"

namespace BAT {

template<>
VariableReader<MultiFloatPointer>::~VariableReader(){
    delete variable;
}

template<>
unsigned int VariableReader<MultiFloatPointer>::size() const{
    throwExceptionIfNotItitialised();
    return variable->size();
}

template<>
unsigned int VariableReader<MultiDoublePointer>::size() const{
    throwExceptionIfNotItitialised();
    return variable->size();
}

template<>
unsigned int VariableReader<MultiIntPointer>::size() const{
    throwExceptionIfNotItitialised();
    return variable->size();
}

template<>
unsigned int VariableReader<MultiBoolPointer>::size() const{
    throwExceptionIfNotItitialised();
    return variable->size();
}

template<>
double VariableReader<MultiFloatPointer>::getVariableAt(unsigned int index) const{
    throwExceptionIfNotItitialised();
    return variable->at(index);
}

template<>
double VariableReader<MultiDoublePointer>::getVariableAt(unsigned int index) const{
    throwExceptionIfNotItitialised();
    return variable->at(index);
}

template<>
int VariableReader<MultiIntPointer>::getIntVariableAt(unsigned int index) const{
    throwExceptionIfNotItitialised();
    return variable->at(index);
}

template<>
unsigned int VariableReader<MultiIntPointer>::getUIntVariableAt(unsigned int index) const{
    throwExceptionIfNotItitialised();
    return variable->at(index);
}
template<>
bool VariableReader<MultiBoolPointer>::getBoolVariableAt(unsigned int index) const{
    throwExceptionIfNotItitialised();
    return variable->at(index);
}
}
