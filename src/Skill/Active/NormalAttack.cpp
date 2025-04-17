#include "NormalAttack.h"

#include "../../Entity/Creature.h"
#include "../../Battle/IBattleInfo.h"

namespace FTK
{
	double NormalAttack::RollProbability() const
	{
		auto master = this->master.lock();
		if (master == nullptr)
			return 0.;

		return static_cast<double>(master->CurrentAttribute().HitRate) / 100.;
	}
}