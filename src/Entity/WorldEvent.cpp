#include "WorldEvent.h"

#include "../Manager/PlayerManager.hpp"

namespace FTK
{
	void WorldEvent::Happen(std::shared_ptr<Player> player, int roll)
	{
		switch (roll)
		{
		case 0:
			player->Hurt(player->ModifyAttribute().HP / 10);
			break;
		case 1:
			[[fallthrough]];
		case 2:
			[[fallthrough]];
		case 3:
			[[fallthrough]];
		case 4:
			PlayerManager::Instance().SetMoney(PlayerManager::Instance().GetMoney() + 2 * roll);
			break;
		default:
			break;
		}
	}
}