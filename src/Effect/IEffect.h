#ifndef IEffect_H
#define IEffect_H

#include <memory>

#include "../GameEvent/EmGameInvokeEvent.h"

namespace FTK
{
	// TODO: Effect manager
	class IBattleInfo;
	class IEffect : public std::enable_shared_from_this<IEffect>
	{
	protected: IEffect() {};

	public: virtual void Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> effectData) = 0;
	};
}

#endif // !IEffect_H