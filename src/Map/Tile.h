#ifndef Tile_H
#define Tile_H

#include <optional>
#include <vector>

#include "../Vector2D.hpp"

namespace FTK
{
	struct Tile
	{
	public: enum class EmType
	{
		Road,
		Grass,
		Water,
		Wall,
		Shop, // TODO: imple as Entity
	};
	public: EmType Type;
	public: Vector2D Position;
	public: Tile(EmType type) : Type(type) { }
	};
}
#endif // !Tile_H