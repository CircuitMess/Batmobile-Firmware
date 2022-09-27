#include "FeedService.h"
#include <Loop/LoopManager.h>
#include "NetworkConfig.h"

#define FRAME_LEN 8

const uint8_t frameStart[FRAME_LEN] = { 0x18, 0x20, 0x55, 0xf2, 0x5a, 0xc0, 0x4d, 0xaa };
const uint8_t frameEnd[FRAME_LEN] = { 0x42, 0x2c, 0xd9, 0xe3, 0xff, 0xa0, 0x11, 0x01 };
const uint8_t frameAck[FRAME_LEN] = { 0xba, 0x14, 0x05, 0xb0, 0x21, 0xa2, 0x03, 0x83 };

FeedService Feed;

void FeedService::connect(){
	if(state != Idle) return;

	state = Connecting;
	server = new AsyncServer(batmobileIP, port);

	Serial.print("batmobile IP address = ");
	Serial.println(WiFi.localIP());

	server->onClient([this](void* arg, AsyncClient* client){

		cam = new Camera();
		cam->initialize(true, 20);

		state = Connected;
		LoopManager::addListener(this);
		controller = client;
		Serial.println("new client connected");

		client->onConnect([this](void*, AsyncClient* server){
			state = Connected;
			Serial.println("client connected to port");
		}, nullptr);

		client->onDisconnect([this](void*, AsyncClient* server){
			//TODO - reconnect to wifi in case of total disconnect
			state = Connecting;
			Serial.println("client disconnected from port");
		});

		client->onError([this](void*, AsyncClient* c, err_t error){
			state = Connecting;
			Serial.print("error occured: ");
			Serial.println(c->errorToString(error));
			connect();
		}, nullptr);

		client->onData([this](void* arg, AsyncClient* server, void* data, size_t len){
			if(len == FRAME_LEN && memcmp((uint8_t*)data, frameAck, FRAME_LEN) == 0){
				canSend = true;
			}
		});

		client->onTimeout([this](void*, AsyncClient*, uint32_t time){
			state = Connecting;
			Serial.printf("timeout error %d passed\n", time);
		}, nullptr);

	}, nullptr);

	server->begin();

	Serial.printf("started server on ip %s port %d\n", batmobileIP.toString().c_str(), port);
}

void FeedService::disconnect(){
	state = Idle;
	delete server;
	delete cam;
	server = nullptr;
	LoopManager::removeListener(this);
}

bool FeedService::sendFrame(const CamFrame& camFrame){

	size_t sent = 0;
	while(!controller->canSend());


	sent = controller->write(reinterpret_cast<const char*>(frameStart), FRAME_LEN);
//	Serial.printf("sent: %zu\n", sent);
	if(!sent){
		printf("Frame start send error\n");
		goto fail;
	}
	while(!controller->canSend());
	sent = controller->write(reinterpret_cast<const char*>(&camFrame.jpgSize), sizeof(camFrame.jpgSize));
//	Serial.printf("sent: %zu\n", sent);
	if(!sent){
		printf("Frame size send error\n");
		goto fail;
	}
	while(!controller->canSend());
	sent = controller->write(reinterpret_cast<const char*>(camFrame.jpg), camFrame.jpgSize);
	if(!sent){
		printf("Frame send error\n");
		goto fail;
	}
	while(!controller->canSend());
	sent = controller->write(reinterpret_cast<const char*>((uint8_t*)&camFrame.motors[0]), sizeof(CamFrame) - sizeof(camFrame.jpg) - sizeof(camFrame.jpgSize));
//	Serial.printf("sent: %zu\n", sent);
	if(!sent){
		printf("supplemental frame data send error\n");
		goto fail;
	}
	while(!controller->canSend());
	sent = controller->write(reinterpret_cast<const char*>(frameEnd), FRAME_LEN);
//	Serial.printf("sent: %zu\n", sent);
	if(!sent){
		printf("Frame end send error\n");
		goto fail;
	}

	goto success;

	fail:
	cam->releaseFrame();
	return false;

	success:
	controller->send();
	cam->releaseFrame();
	return true;
}

FeedService::State FeedService::getState() const{
	return state;
}

void FeedService::loop(uint micros){
	if(state != Connected) return;

	cam->loadFrame();

	while(!controller->canSend() || !canSend){
		delay(50);
	}

	auto f = cam->getFrame();
	CamFrame frame { static_cast<uint32_t>(f->len), f->buf, { 0, 0, 0, 0 }, LineStatus::OffLine, {{ 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }},
					 {{ 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }}, MarkerAction::None };



	bool s = sendFrame(frame);
	if(!s){
		Serial.println("send frame failed");
	}

	canSend = false;

}
