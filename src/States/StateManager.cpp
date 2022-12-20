#include <Communication/Communication.h>
#include <Batmobile.h>
#include "StateManager.h"
#include "IdleState.h"
#include "DriveState.h"
#include "PairState.h"

StateManager::StateManager(){
	Com.addDcListener(this);
	Com.addListener({ ComType::DriveMode, ComType::Shutdown }, this);

	S3.setMode(DriveMode::Idle);
	currentMode = DriveMode::Idle;
}

void StateManager::begin(){
	currentState = std::make_unique<PairState>();
	currentState->start();
}

StateManager::~StateManager(){
	Com.removeDcListener(this);
	Com.removeListener(this);
}

void StateManager::onConnected(){
	if(currentState){
		currentState->stop();
		currentState.reset();
	}

	Audio.play(SPIFFS.open("/SFX/paired.aac"));

	Underlights.setValue({ 0, 0, 0 });
	Underlights.blinkTwice({ 0, 255, 0 });

	const uint32_t m = millis();
	while(millis() - m <= 800){
		Underlights.loop(0);
	}

	/** Controller could immediately send a setState packet. Make sure there isn't a race condition between the resulting state change
	 * and the idleState here. TODO */
	S3.setMode(DriveMode::Idle);
	currentMode = DriveMode::Idle;
	currentState = std::make_unique<IdleState>();
	currentState->start();
}

void StateManager::onDisconnected(){
	Audio.play(SPIFFS.open("/SFX/disconnect.aac"));

	if(currentState){
		currentState->stop();
		currentState.reset();
	}

	S3.setMode(DriveMode::Idle);
	currentMode = DriveMode::Idle;
	currentState = std::make_unique<PairState>();
	currentState->start();
}

void StateManager::onDriveMode(DriveMode mode){
	if(mode == currentMode) return;

	if(currentState){
		currentState->stop();
		currentState.reset();
	}

	S3.setMode(mode);
	currentMode = mode;

	if(mode == DriveMode::Idle){
		currentState = std::make_unique<IdleState>();
	}else{
		currentState = std::make_unique<DriveState>(mode);
	}

	if(currentState){
		currentState->start();
	}
}

void StateManager::onShutdown(){
	Com.sendShutdownAck();
	delay(3000);
	shutdown();
}

void StateManager::shutdown(){

	Com.end();
	adc_power_off();
	WiFi.disconnect(true);
	WiFi.mode(WIFI_OFF);
	btStop();

	Underlights.clear();
	Taillights.clear();
	Headlights.clear();
	Motors.stopAll();
	Audio.stop();
	//TODO - power down S3 as well

	const uint8_t deinitPins[] = { LED_FRONT, LED_BACK, LED_R, LED_G, LED_B, MOTOR_FR_A, MOTOR_FL_A, MOTOR_BR_A, MOTOR_BL_A };
	for(const auto& pin: deinitPins){
		ledcDetachPin(pin);
	}

	esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
	esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
	esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
	esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
	esp_deep_sleep_start();
}
