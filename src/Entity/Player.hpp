#ifndef Player_H
#define Player_H

#include "Creature.h"
#include "../Map/Map.hpp"

namespace FTK
{
	class Player : public Creature
	{
	protected: std::shared_ptr<Map> map;

	public: Player() : Creature() {};
	public: Player(
		const std::string& name, UI::Pixel renderPixel, EmType type, const Vector2D& position,
		const Attribute& attribute, std::shared_ptr<Map> map)
		: Creature(name, renderPixel, type, position, attribute),
		map(map)
	{

	}

	protected: inline bool walkable(const Tile& tile) const
	{
		if (tile.Type == Tile::EmType::Wall)
			return false;
		if (tile.Type == Tile::EmType::Water)
			return false;

		return true;
	}
	public: bool Walk(const Vector2D& direction)
	{
		auto nextPosition = position + direction;
		if (nextPosition.X < 0 || nextPosition.Y < 0)
			return false;
		if (nextPosition.X >= map->GetWidth() || nextPosition.Y >= map->GetHeight())
			return false;

		if (auto option = map->GetTile(nextPosition); !option.has_value())
			return false;
		else
		{
			auto tile = option.value();
			if (!this->walkable(tile))
				return false;
		}

		this->position = nextPosition;

		return true;
	}

	public: inline int GetActionPoint() const
	{
		return this->CurrentAttribute().Speed / 10;
	}
	public: inline double GetActionPointProbability() const
	{
		double speed = static_cast<double>(this->CurrentAttribute().Speed) / 100.;
		if (speed > .9)
			return .9;
		else
			return speed;
	}
	};
}
#endif // !Player_H