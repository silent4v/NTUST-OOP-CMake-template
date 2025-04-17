#ifndef EntityManager_H

#define EntityManager_H

#include <map>
#include <memory>


#include "../Entity/Player.hpp"
#include "../Map/Tile.h"
#include "../Util/Singleton.hpp"

namespace FTK
{
	class EntityManager : public Util::Singleton<EntityManager>
	{
	private: friend class Util::Singleton<EntityManager>;

#pragma region Manager
#pragma endregion

	private: std::vector<std::shared_ptr<Entity>> entities;
	private: std::size_t registeredEntityCount = 0;

	private: EntityManager() = default;
	public: void Init()
	{

	}

	public: std::vector<std::shared_ptr<Entity>> PositionHasEntity(const Vector2D& position)
	{
		std::vector<std::shared_ptr<Entity>> result;
		for (const auto& entity : entities)
		{
			if (entity->Position() == position)
			{
				result.push_back(entity);
			}
		}

		return result;
	}
	public: void AddEntity(std::shared_ptr<Entity> entity)
	{
		entities.push_back(entity);
	}
	public: std::size_t RegisterEntity()
	{
		this->registeredEntityCount++;
		return this->registeredEntityCount;
	}
	public: const std::vector<std::shared_ptr<Entity>>& GetEntities() const
	{
		return entities;
	}
	public: void RemoveEntity(std::shared_ptr<Entity> entity)
	{
		auto it = std::find(entities.begin(), entities.end(), entity);
		if (it != entities.end())
		{
			entities.erase(it);
		}
	}

	};
}
#endif // !EntityManager_H
