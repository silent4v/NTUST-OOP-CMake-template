#ifndef UIManager_H

#define UIManager_H

#include <memory>
#include "../Util/Singleton.hpp"
#include "../UI/Scenes/Scene_TravelMap.h"
#include "../UI/Scenes/Scene_Battle.h"
#include "../Battle/BattleManager.h"

namespace FTK
{
	namespace UI
	{
		class UIManager : public Util::Singleton<UIManager>
		{
		private: friend class Util::Singleton<UIManager>;

		public: std::shared_ptr<Scene_TravelMap> Scene_TravelMap = nullptr;
		public: std::shared_ptr<Scene_Battle> Scene_Battle = nullptr;

		private: std::shared_ptr<IScene> currentScene = nullptr;
		private: UIManager() {}

		private: void awake()
		{
			ConsoleRender::Instance().Clear();
		}
		public: void AwakeTravelMap()
		{
			this->awake();
			this->Scene_TravelMap->Awake();
			this->currentScene = this->Scene_TravelMap;
		}
		public: void AwakeBattle(std::shared_ptr<BattleManager> battleManager)
		{
			this->awake();
			this->Scene_Battle->Awake();
			this->Scene_Battle->SetBattleManager(battleManager);
			this->currentScene = this->Scene_Battle;
		}

		public: void Init(std::shared_ptr<Map> map, const std::vector<std::shared_ptr<Player>>& players)
		{
			this->Scene_TravelMap = std::make_shared<UI::Scene_TravelMap>(map, players);
			this->Scene_Battle = std::make_shared<UI::Scene_Battle>();

			this->AwakeTravelMap();
		}

		public: void Render()
		{
			this->currentScene->Render();
		}
		};
	}
}
#endif // !UIManager_H
