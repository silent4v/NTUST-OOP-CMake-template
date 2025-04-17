#include "Entity.h"

#include "../Manager/EntityManager.hpp"
namespace FTK
{
	Entity::Entity(const std::string& name, UI::Pixel renderPixel, EmType type, const Vector2D& position)
		:
		name(name), renderPixel(renderPixel),
		type(type),
		position(position)
	{
		this->id = EntityManager::Instance().RegisterEntity();
	}
}