/*
 * TestPythonParser.h
 *
 *  Created on: 13 Mar 2012
 *      Author: kreczko
 */

#ifndef TESTPYTHONPARSER_H_
#define TESTPYTHONPARSER_H_

class TestPythonParser {
public:
	TestPythonParser();
	virtual ~TestPythonParser();

	void testParsePythonFile();
	void testParsePythonFileExceptionIfFileNotFound();
	void testParsePythonFileExceptionIfFileNotValidPython();
	void testExctractAttribute();
};

#endif /* TESTPYTHONPARSER_H_ */
