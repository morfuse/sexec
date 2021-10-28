#include "player.h"
#include <morfuse/Script/Context.h>

using namespace mfuse;

EventDef EV_Player_IPrint
(
	"iprint",
	EV_DEFAULT,
	"sI",
	"string bold",
	"prints a string to the player, optionally in bold"
);

uint32_t Player::playerCount = 0;

Player::Player()
{
	this->playerId = playerCount++;
	// set the player's targetname
	this->GetTargetComponent().SetTargetName("player");
}

Player::~Player()
{
}

void Player::EventIPrint(Event& ev)
{
	const ScriptContext& context = ScriptContext::Get();
	const OutputInfo& outInfo = context.GetOutputInfo();
	std::ostream* out = outInfo.GetOutput(outputLevel_e::Output);

	bool isBold = false;
	if (ev.NumArgs() > 1) {
		isBold = ev.GetBoolean(2);
	}

	if (!isBold) {
		*out << "[Player " << playerId << "]: " << ev.GetString(1).c_str() << std::endl;
	}
	else {
		*out << "[Player " << playerId << "], bold: " << ev.GetString(1).c_str() << std::endl;
	}
}

MFUS_CLASS_DECLARATION(SimpleEntity, Player, NULL)
{
	{ &EV_Player_IPrint, &Player::EventIPrint },
	{ NULL, NULL }
};
