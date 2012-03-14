/*
 * DatasetInformation.cpp
 *
 *  Created on: 12 Mar 2012
 *      Author: kreczko
 */

#include "../../interface/Python/DatasetInformation.h"
#include "../../interface/Python/PythonParser.h"

using namespace std;

namespace BAT {

DatasetInformation::DatasetInformation(std::string datasetInformationPythonFile) :
		pythonContent_(PythonParser::parse_python_file(datasetInformationPythonFile)), //
		dataSetInfo_(PythonParser::getAttributeFromPyObject<boost::python::dict>(pythonContent_, "datasetInfo")) {

}

DatasetInformation::~DatasetInformation() {
}

const boost::array<double, DataType::NUMBER_OF_DATA_TYPES> DatasetInformation::getCrossSections() const {
	boost::array<double, DataType::NUMBER_OF_DATA_TYPES> cross_sections;
	for (unsigned int type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		cross_sections[type] = getCrossSection((DataType::value) type);
	}
	return cross_sections;
}

double DatasetInformation::getCrossSection(DataType::value type) const {
	string typestr = DataType::names.at(type);
	boost::python::dict dictForType = PythonParser::getObjectFromPyDictionary<boost::python::dict>(dataSetInfo_,
			typestr);
	double cross_section = PythonParser::getObjectFromPyDictionary<double>(dictForType, "cross-section");

	return cross_section;
}

const boost::array<unsigned long, DataType::NUMBER_OF_DATA_TYPES> DatasetInformation::getArrayOfProcessedEvents() const {
	boost::array<unsigned long, DataType::NUMBER_OF_DATA_TYPES> numberOfProcessedEvents;
	for (unsigned int type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		numberOfProcessedEvents[type] = getNumberOfProcessedEvents((DataType::value) type);
	}
	return numberOfProcessedEvents;
}

unsigned long DatasetInformation::getNumberOfProcessedEvents(DataType::value type) const {
	string typestr = DataType::names.at(type);
	boost::python::dict dictForType = PythonParser::getObjectFromPyDictionary<boost::python::dict>(dataSetInfo_,
			typestr);
	unsigned long numberOfProcessedEvents = PythonParser::getObjectFromPyDictionary<unsigned long>(dictForType,
			"NumberOfProcessedEvents");
	return numberOfProcessedEvents;
}

} /* namespace BAT */
