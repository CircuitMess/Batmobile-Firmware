#include "Lightshow.h"
#include <Batmobile.h>

void Lightshow::start(){
	onStart();
}

void Lightshow::stop(){
	onStop();

	Headlights.clear();
	Taillights.clear();
	Underlights.clear();
}
