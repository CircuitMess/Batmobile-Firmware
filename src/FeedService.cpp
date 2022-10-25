#include "FeedService.h"
#include <Loop/LoopManager.h>
#include "NetworkConfig.h"

const char* tag = "Feed";

#define FRAME_LEN 8
const uint8_t frameStart[FRAME_LEN] = { 0x18, 0x20, 0x55, 0xf2, 0x5a, 0xc0, 0x4d, 0xaa };
const uint8_t frameEnd[FRAME_LEN] = { 0x42, 0x2c, 0xd9, 0xe3, 0xff, 0xa0, 0x11, 0x01 };

FeedService::FeedService(){
	ESP_LOGI(tag, "connecting to  %s @ port %d\n", batmobileIP.toString().c_str(), port);
	if(!udp.connect(batmobileIP, port)){
		ESP_LOGE(tag, "couldnt connect");
		return;
	}
	while(!udp.connected());
	ESP_LOGI(tag, "UDP connected");
}

FeedService::~FeedService(){
	udp.close();
}

void FeedService::sendFrame(const DriveInfo& camFrame){
	const size_t toWrite = camFrame.size() + 2 * FRAME_LEN;
	const auto tempBuf = (uint8_t*)malloc(toWrite);
	const uint packetsNum = (toWrite / CONFIG_TCP_MSS) + 1;

	memcpy(tempBuf, frameStart, FRAME_LEN);
	camFrame.toData(tempBuf + FRAME_LEN);
	memcpy(tempBuf + FRAME_LEN + camFrame.size(), frameEnd, FRAME_LEN);

	size_t written = 0;

	for(int j = 0; j < packetsNum; ++j){
		AsyncUDPMessage message(toWrite);
		if(!message){
			ESP_LOGE(tag, "Couldn't alloc AsyncUDPMessage, size: %d B", toWrite);
			break;
		}



		const size_t messWritten = message.write((uint8_t*) tempBuf + written, toWrite - written);
//		ESP_LOGI(tag, "AsyncUDPMessage size: %d", message.length());
		const size_t udpWritten = udp.sendTo(message, controllerIP, FeedService::port);

		if(udpWritten != messWritten){
			ESP_LOGW(tag, "written to AsyncUDPMessage buffer: %d, written to UDP: %d\n", messWritten, udpWritten);
		}

		written += udpWritten;
	}

	if(written != toWrite){
		ESP_LOGW(tag, "written to UDP: %d, expected: %d\n", written, toWrite);
	}
	ESP_LOGI(tag, "written to udp: %d B", written);

	free(tempBuf);
}
