#pragma once

#include <morfuse/Script/SimpleEntity.h>

class Player : public mfuse::SimpleEntity {
private:
	MFUS_CLASS_PROTOTYPE(Player);

private:
	Player();
	~Player();

	void EventIPrint(mfuse::Event& ev);

private:
	uint32_t playerId;
	static uint32_t playerCount;
};