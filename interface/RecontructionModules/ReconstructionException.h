/*
 * ReconstructionException.h
 *
 *  Created on: 25 Aug 2011
 *      Author: kreczko
 */

#ifndef RECONSTRUCTIONEXCEPTION_H_
#define RECONSTRUCTIONEXCEPTION_H_

#include <stdexcept>

namespace BAT {

class ReconstructionException: public std::runtime_error {
public:
	ReconstructionException(std::string msg):runtime_error("Reconstruction exception: " + msg){

	};
	~ReconstructionException() throw (){

	};
};

} /* namespace BAT */
#endif /* RECONSTRUCTIONEXCEPTION_H_ */

