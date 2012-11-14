/*
 * VariableReader.h
 *
 *  Created on: Jun 24, 2010
 *      Author: lkreczko
 */

#ifndef READER_H_
#define READER_H_
#include <boost/shared_ptr.hpp>
#include "TChain.h"
#include "TBranch.h"
#include "TString.h"
#include <exception>

typedef std::vector<float>* MultiFloatPointer;
typedef std::vector<double>* MultiDoublePointer;
typedef std::vector<int>* MultiIntPointer;
typedef std::vector<bool>* MultiBoolPointer;
typedef std::vector<unsigned int>* MultiUIntPointer;

typedef boost::shared_ptr<TChain> TChainPointer;
namespace BAT {
struct VariableNotFoundException: public std::exception {
	TString msg;
	VariableNotFoundException(TString message) :
			msg(message) {
	}
	~VariableNotFoundException() throw () {
	}

	const char* what() const throw () {
		return msg;
	}
};

struct VariableNotInitialisedException: public std::exception {
	TString msg;
	VariableNotInitialisedException(TString message) :
			msg(message) {
	}
	~VariableNotInitialisedException() throw () {
	}

	const char* what() const throw () {
		return msg;
	}
};

template<typename variableType = unsigned int>
class VariableReader {
public:
	VariableReader() :
			input(), variable(0), variableName("") {

	}

	VariableReader(TChainPointer chain, TString varName) :
			input(chain), variable(0), variableName(varName) {

	}

	~VariableReader() {
	}

	const variableType& getVariable() {
		return variable;
	}

	double getVariableAt(unsigned int index) const;
	int getIntVariableAt(unsigned int index) const;
	unsigned int getUIntVariableAt(unsigned int index) const;
	bool getBoolVariableAt(unsigned int index) const;
	unsigned int size() const;

	void initialise() {
		if (doesVariableExist()) {
			enableVariable();
			readVariableFromInput();
		} else
			throw VariableNotFoundException(
					"Variable '" + variableName + "' was not found.\n"
							+ "Please check the file content and/or NTuple version.");
	}

	void initialiseBlindly() {
		enableVariable();
		readVariableFromInput();
	}

	bool doesVariableExist() {
		return input->GetBranch(variableName) != NULL;
	}

	void throwExceptionIfNotItitialised() const {
		if (variable == 0)
			throw VariableNotFoundException("Trying to access variable '" + variableName + "' before initialisation.");
	}
protected:
	TChainPointer input;
	variableType variable;
	TString variableName;

	void readVariableFromInput() {
		input->SetBranchAddress(variableName, &variable);
	}

	void enableVariable() {
		input->SetBranchStatus(variableName, true);
	}
};

}

#endif /* READER_H_ */
