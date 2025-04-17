#ifndef GameState_H
#define GameState_H

#include "../Util/BitFlags.hpp"

namespace FTK
{
	enum class EmGameState
	{
		//StartMenu,
		TravelMap = 1 << 1,
		Shop = 1 << 2,
		OpenBag = 1 << 3,
		InteractAndShowDetail = 1 << 4,
		TeleportScroll_FreeTravelOnMap = 1 << 5,
		InteractWithWorldEvent = 1 << 6,

		Battle = 1 << 7,
		BattleSelectSkill = 1 << 8,
		BattleSelectTarget = 1 << 9,
		BattleUseFocus = 1 << 10,
		BattleSelectItem = 1 << 11,
		BattleWaitingConfirm = 1 << 12,

	};
	class GameState : protected Util::BitFlags<EmGameState>
	{
	public: GameState(EmGameState state) : BitFlags<EmGameState>(state) {}
	public: inline EmGameState State() const { return this->flags; }
	public: inline void ChangeState(EmGameState state) { this->setFlag(state); }
	public: inline void RemoveState(EmGameState state) { this->removeFlag(state); }
	public: inline void AddState(EmGameState state) { this->addFlag(state); }
	public: inline bool HasState(EmGameState state) const { return this->hasFlag(state); }
	};
}
#endif // !GameState_H