#ifndef EmGameInvokeEvent_H
#define EmGameInvokeEvent_H

namespace FTK
{
	enum class EmGameInvokeEvent
	{
		Battle_Start,
		Battle_TurnStart,
		//Battle_WhenAttack,
		Battle_WhenDefend,
		Battle_BeHurt,
		Battle_CauseHarm,
		Battle_TurnEnd,
		Battle_End,
		Battle_UsingSkill,

		Always,
	};
}
#endif // !EmGameInvokeEvent_H