#ifndef BattleManager_H
#define BattleManager_H

#include <memory>
#include <vector>

#include "../Battle/IBattleInfo.h"
#include "../Util/Event.hpp"
#include "ProcessController.hpp"

namespace FTK
{
	class BattleManager
	{
#pragma region using
	protected:
		using ProcessController = Battle::ProcessController;
#pragma endregion

	public: std::shared_ptr<BattleEntities> battleEntities = nullptr;
	public: std::shared_ptr<ProcessController> processController = nullptr;
	public: BattleManager(std::shared_ptr<BattleEntities> battleEntities);

	public: Util::Event<void, std::shared_ptr<Creature>> Event_WaitingCreatureSelectAction;
	public: Util::Event<void, std::shared_ptr<IBattleInfo>> Event_OnBattleResult;

	public: void Battle(std::shared_ptr<ActiveBattleInfo> battleInfo);

	public: void Process();
	};
}
#endif // !BattleManager_H
