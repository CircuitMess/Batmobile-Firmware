#ifndef BATMOBILE_FIRMWARE_LIGHTSHOW_H
#define BATMOBILE_FIRMWARE_LIGHTSHOW_H

#include <Batmobile.h>

enum class LightshowType : uint8_t {
	None, SolidFrontBack, BreatheR, BreatheG, FrontAndFire, BreatheB, Rainbow, BreatheRFrontBack, Police, Size
};

class Lightshow {
public:
	virtual ~Lightshow();
	void start();
	void stop();
	static std::unique_ptr<Lightshow> createLightshow(LightshowType type);
protected:
	virtual void onStart(){};

	virtual void onStop(){};
};


#endif //BATMOBILE_FIRMWARE_LIGHTSHOW_H
