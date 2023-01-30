#include "Driver.h"

Driver::Driver(DriveMode mode) : mode(mode){

}

Driver::~Driver(){
	if(started){
		throw "Driver destructing while still started";
	}
}

DriveMode Driver::getMode() const{
	return mode;
}

void Driver::start(){
	if(started) return;
	started = true;
	onStart();
	controls.start();
}

void Driver::stop(){
	if(!started) return;
	started = false;
	onStop();
	controls.stop();
}

void Driver::onStart(){ }

void Driver::onStop(){ }
