#ifndef BATCONTROLLER_FIRMWARE_MODEL_HPP
#define BATCONTROLLER_FIRMWARE_MODEL_HPP

#include <Arduino.h>

enum LineStatus{
	OnLine, OffLine
};

struct Circle{
	int16_t x;
	int16_t y;
	uint16_t r;
};

struct Marker{
	int16_t x;
	int16_t y;
	int32_t id;
};
enum MarkerAction {
	None,
	Forward,           //Forward
	Backward,          //Backward
	Do360,			   //360
	Burnout,           //Burnout
	HeadlightOn,       //Headlight  ledice ON
	HeadlightOff,      //Headlight ledice OFF
	Honk,              //Potrubi
	Batsplosion,       //zvuk + svijetlo
	RGBOff,            //RGB donje ledice OFF
	RGBBreathe,        //RGB donje ledice breathing
	RGBSolid,          //RGB donje ledice solid
	Bats,              //Zvuk šišmiša + na ekranu šišmiši
};



struct CamFrame{
	uint32_t jpgSize;
	uint8_t* jpg;
	int8_t motors[4];
	LineStatus lineStatus;
	Circle ballsDetected[4];
	Marker markers[4];
	MarkerAction markerAction;
};



#endif //BATCONTROLLER_FIRMWARE_MODEL_HPP
