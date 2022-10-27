#ifndef BATMOBILE_FIRMWARE_STATE_H
#define BATMOBILE_FIRMWARE_STATE_H

class State {
public:
	virtual ~State() = default;

	void start();
	void stop();
	bool isStarted() const;

	static void pairState();
	static void driveState();
	static void idleState();


protected:
	virtual void onStart(){};
	virtual void onStop(){};

private:
	static State* currentState;

	bool started = false;
};


#endif //BATMOBILE_FIRMWARE_STATE_H