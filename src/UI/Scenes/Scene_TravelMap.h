#ifndef Scene_TravelMap_H
#define Scene_TravelMap_H

#include <memory>
#include <optional>
#include <string>

#include "../../Define/Color.h"
#include "../../Entity/Player.hpp"
#include "../../Entity/WorldEvent.h"
#include "../../Item/Item.h"
#include "../../Item/Items.h"
#include "../../Item/Shop.h"
#include "../../Map/Map.hpp"
#include "../../Util/LoDash/LoDash.hpp"
#include "../Panel.h"
#include "../Pixel.hpp"
#include "IScene.h"

namespace FTK
{
	namespace UI
	{
		class Scene_TravelMap : public IScene
		{
		public: static const int Panel_MAP_WIDTH = 101;
		public: static const int Panel_MAP_HEIGHT = 35;
		private: UIPanel panelMap = UI::UIPanel(Vector2D(1, 1), Panel_MAP_WIDTH, Panel_MAP_HEIGHT);
		public: static const int Panel_DetailConsole_WIDTH = 42;
		public: static const int Panel_DetailConsole_HEIGHT = 35;
		private: UIPanel panelDetailConsole = UI::UIPanel(Vector2D(101, 1), Panel_DetailConsole_WIDTH, Panel_DetailConsole_HEIGHT);
		public: static const int Panel_PlayerInfo_WIDTH = 40;
		public: static const int Panel_PlayerInfo_HEIGHT = 16;
		private: UIPanel panelPlayerInfos[3]{
			UI::UIPanel(Vector2D(5, 35), Panel_PlayerInfo_WIDTH, Panel_PlayerInfo_HEIGHT),
			UI::UIPanel(Vector2D(50, 35), Panel_PlayerInfo_WIDTH, Panel_PlayerInfo_HEIGHT),
			UI::UIPanel(Vector2D(95, 35), Panel_PlayerInfo_WIDTH, Panel_PlayerInfo_HEIGHT)
		};

		private: std::shared_ptr<Map> map;
		private: std::vector<std::shared_ptr<Player>> players;

		public: Vector2D MapCenterPosition = Vector2D(0, 0);

		public: static inline int TileToForeGroundColor(const Tile::EmType& type)
		{
			switch (type)
			{
			case Tile::EmType::Shop:
				return Util::ForeGroundColor(EmColor::Black);
			case Tile::EmType::Road:
				return Util::ForeGroundColor(EmColor::Black);
			default:
				return Util::ForeGroundColor(EmColor::White);
			}
		}
		public: static inline int TileToForeGroundColor(const Tile& tile)
		{
			return TileToForeGroundColor(tile.Type);
		}
		public: static inline int TileToBackGroundColor(const Tile::EmType& type)
		{
			switch (type)
			{
			case Tile::EmType::Road:
				return Util::BackGroundColor(EmColor::Brown);
			case Tile::EmType::Grass:
				return Util::BackGroundColor(EmColor::Green);
			case Tile::EmType::Water:
				return Util::BackGroundColor(EmColor::Blue);
			case Tile::EmType::Wall:
				return Util::BackGroundColor(EmColor::DarkGray);
			case Tile::EmType::Shop:
				return Util::BackGroundColor(EmColor::Yellow);
			default:
				return Util::BackGroundColor(EmColor::Black);
			}
		}
		public: static inline int TileToBackGroundColor(const Tile& tile)
		{
			return TileToBackGroundColor(tile.Type);
		}
		public: static inline wchar_t TileToCharacter(const Tile::EmType& type)
		{
			switch (type)
			{
			case Tile::EmType::Road:
				return L'.';
			case Tile::EmType::Grass:
				return L'.';
			case Tile::EmType::Water:
				return L'~';
			case Tile::EmType::Wall:
				return L' ';
			case Tile::EmType::Shop:
				return L'$';
			default:
				return L' ';
			}
		}
		public: static inline wchar_t TileToCharacter(const Tile& tile)
		{
			return TileToCharacter(tile.Type);
		}
		public: static inline std::string TileToCharacterString(const Tile::EmType& type)
		{
			std::wstring wStr = std::wstring(1, TileToCharacter(type));
			return std::string(wStr.begin(), wStr.end());
		}
		private: void renderMap();

#pragma region DetailConsole

		public: class IDetailConsole
		{
		public: virtual std::vector<DrawInfo> ToString() const = 0;
		};

		public: class WhenTravelMap : public IDetailConsole
		{
		public: virtual std::vector<DrawInfo> ToString() const override;
		};

		public: class InteractWithEnemy : public IDetailConsole
		{
		private: std::vector<std::shared_ptr<Player>> players;
		private: std::vector<std::shared_ptr<Creature>> enemys;
		public: enum class EmAction
		{
			Attack,
			Back
		};
		public: EmAction SelectAction;
		public: static const int ACTION_COUNT = 2;

		public: InteractWithEnemy(
			std::vector<std::shared_ptr<Player>> players,
			std::vector<std::shared_ptr<Creature>> enemys,
			EmAction selectIndex = EmAction::Attack) :
			players(players), enemys(enemys), SelectAction(selectIndex)
		{

		}

		public: virtual std::vector<DrawInfo> ToString() const override;
		};

		public: class InteractWithShop : public IDetailConsole
		{
		public: FTK::Shop Shop;
		public: int SelectIndex = 0;
		public: InteractWithShop(const FTK::Shop& shop) :
			Shop(shop)
		{

		}
		public: virtual std::vector<DrawInfo> ToString() const override;
		};

		public: class InteractWithBag : public IDetailConsole
		{
		private: std::vector<std::shared_ptr<Item>> itemList;
		public: std::map<Item::EmType, int> BagItems;
		public: int SelectIndex = 0;
		public: InteractWithBag(const std::map<Item::EmType, int>& bagItems) :
			BagItems(bagItems)
		{
			this->itemList = LoDash::Map(
				LoDash::Keys(this->BagItems),
				[](const Item::EmType& type) -> std::shared_ptr<Item>
				{
					return NewItem(type);
				}
			);
		}
		public: virtual std::vector<DrawInfo> ToString() const override;
		};

		public: class InteractWithWorldEvent : public IDetailConsole
		{
		public: std::shared_ptr<Creature> Player;
		public: std::shared_ptr<FTK::WorldEvent> WorldEvent;
		public: int FocusAmount = 0;

		public: int RollResult = -1;
		public: InteractWithWorldEvent(
			std::shared_ptr<Creature> player,
			std::shared_ptr<FTK::WorldEvent> worldEvent) :
			WorldEvent(worldEvent), Player(player)
		{

		}
		public: virtual std::vector<DrawInfo> ToString() const override;
		};

		public: class DetailConsole
		{
		public: std::shared_ptr<IDetailConsole> CurrentConsole = nullptr;
		public: std::shared_ptr<InteractWithEnemy> InteractWithEnemy = nullptr;
		public: std::shared_ptr<InteractWithShop> InteractWithShop = nullptr;
		public: std::shared_ptr<InteractWithBag> InteractWithBag = nullptr;
		public: std::shared_ptr<WhenTravelMap> WhenTravelMap = nullptr;
		public: std::shared_ptr<InteractWithWorldEvent> InteractWithWorldEvent = nullptr;

		public: inline void Reset()
		{
			this->CurrentConsole = nullptr;
			this->InteractWithEnemy = nullptr;
			this->InteractWithShop = nullptr;
			this->InteractWithBag = nullptr;
			this->WhenTravelMap = nullptr;
		}

		public: inline void SetInteractWithEnemy(std::shared_ptr<Scene_TravelMap::InteractWithEnemy> interactWithEnemy)
		{
			this->InteractWithEnemy = interactWithEnemy;
			this->CurrentConsole = interactWithEnemy;
		}
		public: inline void ChangeInteractWithEnemySelectAction(int offset)
		{
			if (this->InteractWithEnemy == nullptr)
				return;
			if (int(this->InteractWithEnemy->SelectAction) + offset >= InteractWithEnemy::ACTION_COUNT)
				return;
			if (int(this->InteractWithEnemy->SelectAction) + offset < 0)
				return;

			this->InteractWithEnemy->SelectAction = InteractWithEnemy::EmAction(int(this->InteractWithEnemy->SelectAction) + offset);
		}


		public: inline void SetInteractWithShop(std::shared_ptr<Scene_TravelMap::InteractWithShop> interactWithShop)
		{
			this->InteractWithShop = interactWithShop;
			this->CurrentConsole = interactWithShop;
		}
		public: inline void ChangeInteractWithShopSelectIndex(int offset)
		{
			if (this->InteractWithShop == nullptr)
				return;
			if (this->InteractWithShop->SelectIndex + offset >= this->InteractWithShop->Shop.ItemMappingPrice.size())
				return;
			if (this->InteractWithShop->SelectIndex + offset < 0)
				return;

			this->InteractWithShop->SelectIndex += offset;
		}
		public: inline std::optional<Shop::ItemAndPrice> GetInteractWithShopSelectedItem() const
		{
			if (this->InteractWithShop == nullptr)
				return std::nullopt;
			if (this->InteractWithShop->SelectIndex >= this->InteractWithShop->Shop.ItemMappingPrice.size())
				return std::nullopt;
			if (this->InteractWithShop->SelectIndex < 0)
				return std::nullopt;

			auto itemList = LoDash::Values(this->InteractWithShop->Shop.ItemMappingPrice);
			return itemList[this->InteractWithShop->SelectIndex];
		}

		public: inline void SetInteractWithBag(std::shared_ptr<Scene_TravelMap::InteractWithBag> interactWithBag)
		{
			this->InteractWithBag = interactWithBag;
			this->CurrentConsole = interactWithBag;
		}
		public: inline void ChangeInteractWithBagSelectIndex(int offset)
		{
			if (this->InteractWithBag == nullptr)
				return;
			if (this->InteractWithBag->SelectIndex + offset >= this->InteractWithBag->BagItems.size())
				return;
			if (this->InteractWithBag->SelectIndex + offset < 0)
				return;

			this->InteractWithBag->SelectIndex += offset;
		}
		public: inline std::shared_ptr<Item> GetInteractWithBagSelectedItem() const
		{
			if (this->InteractWithBag == nullptr)
				return nullptr;
			if (this->InteractWithBag->SelectIndex >= this->InteractWithBag->BagItems.size())
				return nullptr;
			if (this->InteractWithBag->SelectIndex < 0)
				return nullptr;

			auto itemList = LoDash::Keys(this->InteractWithBag->BagItems);
			return NewItem(itemList[this->InteractWithBag->SelectIndex]);
		}

		public: inline void SetInteractWithWorldEvent(std::shared_ptr<Scene_TravelMap::InteractWithWorldEvent> interactWithWorldEvent)
		{
			this->InteractWithWorldEvent = interactWithWorldEvent;
			this->CurrentConsole = interactWithWorldEvent;
		}
		public: inline void ChangeInteractWithWorldEventFocusAmount(int offset)
		{
			if (this->InteractWithWorldEvent == nullptr)
				return;
			if (this->InteractWithWorldEvent->FocusAmount + offset < 0)
				return;
			if (this->InteractWithWorldEvent->FocusAmount + offset > this->InteractWithWorldEvent->Player->CurrentAttribute().Focus)
				return;
			if (this->InteractWithWorldEvent->FocusAmount + offset > this->InteractWithWorldEvent->WorldEvent->RollAmount())
				return;

			this->InteractWithWorldEvent->FocusAmount += offset;
		}

		public: inline void SetWhenTravelMap()
		{
			auto whenTravelMapPtr = std::make_shared<Scene_TravelMap::WhenTravelMap>();
			this->WhenTravelMap = whenTravelMapPtr;
			this->CurrentConsole = whenTravelMapPtr;
		}

		public: inline std::vector<DrawInfo> ToString() const
		{
			if (this->CurrentConsole == nullptr)
				return {};

			return this->CurrentConsole->ToString();
		}
		} DetailConsole;

		private: void renderDetailConsole();
#pragma endregion

		private: void renderPlayerInfos();

		private: void focusPanel(UIPanel& panel)
		{
			panel.DrawEdge(PixelColor(EmColor::Green, EmColor::Black));
		}
		public: void CleanFocusPanel()
		{
			this->panelMap.DrawEdge();
			this->panelDetailConsole.DrawEdge();
			for (int i = 0; i < 3; i++)
				this->panelPlayerInfos[i].DrawEdge();
		}

		public: Scene_TravelMap(std::shared_ptr<Map> map, const std::vector<std::shared_ptr<Player>>& players) :
			map(map), players(players)
		{

		}
		public: virtual void Awake() override;
		public: virtual void Render() override;
		};
	}
}
#endif // !Scene_TravelMap_H