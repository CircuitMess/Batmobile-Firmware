#ifndef BATMOBILE_FIRMWARE_FEEDSERVICE_H
#define BATMOBILE_FIRMWARE_FEEDSERVICE_H

#include "Model.hpp"
#include <Loop/LoopListener.h>
#include <Camera.h>
#include <AsyncTCP.h>

class FeedService : private LoopListener{
public:
	void connect();
	void disconnect();

	enum State {
		Idle, Connecting, Connected
	};

	State getState() const;

	void loop(uint micros) override;

private:

	bool sendFrame(const CamFrame& camFrame);

	Camera *cam;
	AsyncServer* server;

	State state = Idle;

	const uint16_t port = 5000;

	AsyncClient* controller;

	volatile bool canSend = true; //ack is pending or not
};

extern FeedService Feed;

#endif //BATMOBILE_FIRMWARE_FEEDSERVICE_H
