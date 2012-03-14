/*
 * MCInformation.h
 *
 *  Created on: 12 Mar 2012
 *      Author: kreczko
 */

#ifndef MCINFORMATION_H_
#define MCINFORMATION_H_
#include "../DataTypes.h"

#include <string>

#include <boost/array.hpp>
#include <boost/python.hpp>
#include <boost/python/dict.hpp>



namespace BAT {

class DatasetInformation {
public:
	DatasetInformation(std::string datasetInformationPythonFile);
	virtual ~DatasetInformation();
	const boost::array<double, DataType::NUMBER_OF_DATA_TYPES> getCrossSections() const;
	const boost::array<unsigned long, DataType::NUMBER_OF_DATA_TYPES> getArrayOfProcessedEvents() const;
	double getCrossSection(DataType::value) const;
	unsigned long getNumberOfProcessedEvents(DataType::value) const;
private:
	boost::python::object pythonContent_;
	boost::python::dict dataSetInfo_;

	void readPythonFile();
};

} /* namespace BAT */
#endif /* MCINFORMATION_H_ */
