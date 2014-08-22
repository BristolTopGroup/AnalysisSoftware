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

#ifndef CUTE_TIMING_SIGNALER_H_
#define CUTE_TIMING_SIGNALER_H_
#include "cute_listener.h"
// TODO: not yet working on windows..... BROKEN...
// TODO: should use a system (independent) type for hires-timer
namespace cute {
struct timing_listener_helper{
	long long laststart;
	void start();
	long long stop();
	std::string timingMessage(char const *msg,long long time);
};
template <typename Listener=null_listener>
struct timing_listener:Listener{
	timing_listener()
	:Listener()
	,currentTest(0),allTests(0){}
	timing_listener(Listener const &s)
	:Listener(s)
	,currentTest(0),allTests(0){}
	void start(test const &t){
		Listener::start(t);
		timer.start();
	}
	void taketime(){
		currentTest= timer.stop();
		allTests+=currentTest;
	}
	void success(test const &t, char const *msg){
		taketime();
		Listener::success(t,timer.timingMessage(msg,currentTest).c_str());
	}
	void failure(test const &t,test_failure const &e){
		taketime();
		Listener::failure(t,e);
	}
	void error(test const &t,char const *what){
		taketime();
		Listener::error(t,what);
	}
	timing_listener_helper timer;
	long long currentTest;
	long long allTests;
};
}
#endif /*CUTE_TIMING_SIGNALER_H_*/
