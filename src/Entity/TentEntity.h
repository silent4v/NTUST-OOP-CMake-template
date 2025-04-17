#ifndef Tent_H
#define Tent_H

#include "Entity.h"

namespace FTK
{
	class Creature;
	class TentEntity : public Entity
	{
	private: std::shared_ptr<Creature> master;
	public: TentEntity(std::shared_ptr<Creature> master);
	};
}

#endif // Tent_H
