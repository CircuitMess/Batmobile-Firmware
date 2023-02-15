#include "Lightshow.h"
#include <Batmobile.h>

Lightshow::~Lightshow(){
	stop();
}

void Lightshow::start(){
	onStart();
}

void Lightshow::stop(){
	onStop();

	Headlights.clear();
	Taillights.clear();
	Underlights.clear();
}
