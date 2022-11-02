#include "Feed.h"
#include <Loop/LoopManager.h>
#include <NetworkConfig.h>

const char* tag = "Feed";

Feed::Feed() : txBuf(static_cast<uint8_t*>(malloc(BufSize))){
	memset(txBuf, 0, BufSize);
}

Feed::~Feed(){
	free(txBuf);
}

void Feed::sendFrame(const DriveInfo& frame){
	auto frameSize = frame.size();
	auto sendSize = frameSize + sizeof(FrameHeader) + sizeof(FrameTrailer) + sizeof(size_t);
	if(sendSize > BufSize){
		ESP_LOGW(tag, "Data frame buffer than send buffer. %zu > %zu\n", sendSize, BufSize);
		return;
	}

	size_t cursor = 0;
	auto addData = [&cursor, this](const void* data, size_t size){
		memcpy(txBuf + cursor, data, size);
		cursor += size;
	};

	addData(FrameHeader, sizeof(FrameHeader));
	addData(&frameSize, sizeof(size_t));
	frame.toData(txBuf + cursor); cursor += frameSize;
	addData(FrameTrailer, sizeof(FrameTrailer));

	size_t sent = 0;
	while(sent < sendSize){
		size_t sending = std::min((size_t) (CONFIG_TCP_MSS), sendSize - sent);
		udp.writeTo(txBuf + sent, sending, controllerIP, feedPort);
		sent += sending;
	}

	memset(txBuf, 0, sendSize);
}
