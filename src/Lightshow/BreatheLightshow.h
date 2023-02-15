#ifndef BATMOBILE_FIRMWARE_BREATHELIGHTSHOW_H
#define BATMOBILE_FIRMWARE_BREATHELIGHTSHOW_H

#include "Lightshow.h"

template<LightshowType type>
class BreatheLightshow : public Lightshow {
protected:
	void onStart() override{
		switch(type){
			case LightshowType::BreatheR:
				Underlights.breathe({ 0, 0, 0 }, { 255, 0, 0 }, breathePeriod);
				break;
			case LightshowType::BreatheG:
				Underlights.breathe({ 0, 0, 0 }, { 0, 255, 0 }, breathePeriod);
				break;
			case LightshowType::BreatheB:
				Underlights.breathe({ 0, 0, 0 }, { 0, 0, 255 }, breathePeriod);
				break;
			case LightshowType::BreatheRFrontBack:
				Underlights.breathe({ 0, 0, 0 }, { 255, 0, 0 }, breathePeriod);
				Headlights.breathe(0, 255, breathePeriod);
				Taillights.breathe(0, 255, breathePeriod);
				break;
			default:
				break;
		}
	}

private:
	static constexpr uint32_t
	breathePeriod = 4000;
};


#endif //BATMOBILE_FIRMWARE_BREATHELIGHTSHOW_H
