#ifndef Map_H
#define Map_H

#include <optional>
#include <vector>

#include "../Vector2D.hpp"
#include "Tile.h"

namespace FTK
{
	class Map
	{
	private: std::vector<std::vector<Tile>> tiles;

	public: Map(std::vector<std::vector<Tile>> tiles) : tiles(tiles)
	{

	}

	public: inline int GetWidth() const
	{
		return tiles[0].size();
	}
	public: inline int GetHeight() const
	{
		return tiles.size();
	}
	public: inline std::optional<Tile> GetTile(int x, int y) const
	{
		if (x < 0 || x >= GetWidth() || y < 0 || y >= GetHeight())
			return std::nullopt;

		return tiles[y][x];
	}
	public: inline std::optional<Tile> GetTile(Vector2D position) const
	{
		return this->GetTile(position.X, position.Y);
	}
	public: inline const std::vector<std::vector<Tile>>& GetTiles() const
	{
		return tiles;
	}
	};
}
#endif // !Map_H