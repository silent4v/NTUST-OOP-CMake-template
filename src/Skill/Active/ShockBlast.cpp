#include "ShockBlast.h"

#include "../../Entity/Creature.h"

namespace FTK
{
	double ShockBlast::RollProbability() const
	{
		auto master = this->master.lock();
		if (master == nullptr)
		{
			return 0.0;
		}

		double probability = static_cast<double>(master->CurrentAttribute().HitRate) - 5.;

		return probability / 100.;
	}
}