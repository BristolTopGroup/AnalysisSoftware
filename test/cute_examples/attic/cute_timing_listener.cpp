/*********************************************************************************
 * This file is part of CUTE.
 *
 * CUTE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CUTE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CUTE.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2006 Peter Sommerlad
 *
 *********************************************************************************/

#include "cute_timing_listener.h"
#include <time.h>
#include <sstream>
namespace cute {
// TODO: BROKEN...
void timing_listener_helper::start(){
	laststart= clock();
}
long long  timing_listener_helper::stop()
{
	return clock()-laststart;
}
std::string  timing_listener_helper::timingMessage(char const *msg,long long time){
			std::ostringstream os;
	os << msg << " took " << time << " ms";
	return os.str();
}
}
