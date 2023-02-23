#ifndef BATMOBILE_FIRMWARE_LIGHTSHOW_H
#define BATMOBILE_FIRMWARE_LIGHTSHOW_H

#include <Batmobile.h>

enum class LightshowType : uint8_t {
	None, SolidFrontBack, BreatheR, BreatheG, BreatheB, Rainbow, BreatheRFrontBack, FrontAndFire, Police, Size
};

class Lightshow {
public:
	virtual ~Lightshow();
	void start();
	void stop();

protected:
	virtual void onStart(){};

	virtual void onStop(){};
};


#endif //BATMOBILE_FIRMWARE_LIGHTSHOW_H
