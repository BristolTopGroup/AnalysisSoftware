#include "../interface/DataTypes.h"
#include <string>
namespace BAT {
DataType::value DataType::getDataType(const std::string filename) {
	DataType::value filetype = DataType::ElectronHad;

	for (unsigned int index = 0; index < DataType::names.size(); ++index) {
		const std::string searchString(DataType::names.at(index));

		if (filename.find(searchString) != std::string::npos) {
			filetype = (DataType::value) index;
		}
	}
	return filetype;
}
}
