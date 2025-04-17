#include "TentEntity.h"

#include "../Entity/Creature.h"
#include "../Manager/EntityManager.hpp"
#include "../Manager/GameStateController.hpp"

namespace FTK
{
	TentEntity::TentEntity(std::shared_ptr<Creature> master) :
		Entity(
			"Tent",
			UI::Pixel{ L'T', UI::PixelColor(Util::ForeGroundColor(EmColor::White)) },
			EmType::Tent,
			master->Position()
		), master(master)
	{
		GameStateController::Instance().Event_PlayerAction.Register(
			[&](FTK::UserTravelMapAction userAction)
			{
				if (userAction.Player == this->master)
				{
					EntityManager::Instance().RemoveEntity(this->shared_from_this());
				}
			}
		);
		GameStateController::Instance().Event_PlayerEndAction.Register(
			[&](std::shared_ptr<Player> player)
			{
				if (player->Position() != this->Position())
				{
					return;
				}

				player->Heal(50);
				player->RestoreFocus(5);
			}
		);
	}
}