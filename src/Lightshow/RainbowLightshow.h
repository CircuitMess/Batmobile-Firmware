#ifndef BATMOBILE_FIRMWARE_RAINBOWLIGHTSHOW_H
#define BATMOBILE_FIRMWARE_RAINBOWLIGHTSHOW_H

#include "Lightshow.h"
#include <Loop/LoopListener.h>

class RainbowLightshow : public Lightshow, private LoopListener {
protected:
	void onStart() override;
	void onStop() override;
private:
	void loop(uint micros) override;

	uint32_t timer = 0;
	static constexpr uint32_t fadeTime = 6000000; //6s fade cycle

};


#endif //BATMOBILE_FIRMWARE_RAINBOWLIGHTSHOW_H
