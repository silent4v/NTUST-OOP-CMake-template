#ifndef Entity_H
#define Entity_H

#include <memory>
#include <vector>

#include "../UI/Pixel.hpp"
#include "../Vector2D.hpp"

namespace FTK
{
	class Entity : public std::enable_shared_from_this<Entity>
	{
	public: enum class EmType
	{
		Enemy,
		Player,
		Tent,
		WorldEvent,
	};

	private: UI::Pixel renderPixel;
	public: inline const UI::Pixel& RenderPixel() const { return this->renderPixel; }
	private: std::string name;
	public: inline const std::string& Name() const { return this->name; }
	private: EmType type;
	public: inline const EmType& Type() const { return this->type; }
	protected: Vector2D position;
	public: inline const Vector2D& Position() const { return this->position; }
	public: inline void SetPosition(const Vector2D& position) { this->position = position; }

	private: std::size_t id;
	public: inline const std::size_t& ID() const { return this->id; }
	public: bool Dead = false;

	public: Entity() = default;
	public: Entity(const std::string& name, UI::Pixel renderPixel, EmType type, const Vector2D& position);
	};
}
#endif // !Entity_H