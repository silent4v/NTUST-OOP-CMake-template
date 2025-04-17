#include "Items.h"

#include "../Entity/Creature.h"
#include "../Entity/TentEntity.h"
#include "../Manager/EntityManager.hpp"
#include "../Manager/GameManager.hpp"
#include "../Manager/GameStateController.hpp"
#include "../Util/LoDash/LoDash.hpp"

namespace FTK::item
{
	bool Godsbeard::Use(std::shared_ptr<Creature> master)
	{
		master->Heal(25);

		return true;
	}

	bool GoldenRoot::Use(std::shared_ptr<Creature> master)
	{
		master->RestoreFocus(3);

		return true;
	}

	bool TeleportScroll::Use(std::shared_ptr<Creature> master)
	{
		GameManager::Instance().StartFreeTravelMap(master->Position());

		return true;
	}

	bool Tent::Use(std::shared_ptr<Creature> master)
	{
		// check already has tent
		auto canUseTent = LoDash::All(EntityManager::Instance().GetEntities(),
			[&](const std::shared_ptr<Entity>& entity)
			{
				if (entity->Type() == Entity::EmType::Tent && entity->Position() == master->Position())
				{
					return false;
				}
				else if (entity->Type() == Entity::EmType::Enemy && entity->Position() == master->Position())
				{
					return false;
				}

				return true;
			}
		);

		if (auto tile = PlayerManager::Instance().Map->GetTile(master->Position()); tile.has_value())
		{
			if (!(tile->Type == Tile::EmType::Road || tile->Type == Tile::EmType::Grass))
			{
				canUseTent = false;
			}
		}

		if (!canUseTent)
		{
			return false;
		}

		auto tent = std::make_shared<TentEntity>(master);
		EntityManager::Instance().AddEntity(tent);

		return true;
	}
}