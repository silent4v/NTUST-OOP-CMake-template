#ifndef GameManager_H

#define GameManager_H

#include <memory>
#include <random>

#include "../Battle/BattleManager.h"
#include "EntityManager.hpp"
#include "GameStateController.hpp"
#include "PlayerManager.hpp"
#include "UIManager.hpp"
#include "UserEvent.h"

#include "../AI/AIAgent.hpp"
#include "../Entity/Player.hpp"
#include "../Entity/WorldEvent.h"
#include "../Item/Equipments/Equipments.h"
#include "../Item/Items.h"
#include "../Item/Shop.h"
#include "../Map/Tile.h"
#include "../Skill/Skills.h"
#include "../Util/LoDash/LoDash.hpp"
#include "../Util/Singleton.hpp"

namespace FTK
{
	class GameManager : public Util::Singleton<GameManager>
	{
	private: friend class Util::Singleton<GameManager>;

#pragma region Manager
	public: UI::UIManager* uiManager = &UI::UIManager::Instance();
	public: UserEvent* userEvent = &UserEvent::Instance();
	public: GameStateController* gameStateController = &GameStateController::Instance();
	public: EntityManager* entityManager = &EntityManager::Instance();
	public: PlayerManager* playerManager = &PlayerManager::Instance();
#pragma endregion

	private: std::shared_ptr<Map> map;
	private: std::vector<std::shared_ptr<Player>> players;
	private: std::shared_ptr<BattleManager> battleManager = nullptr;
	private: inline std::shared_ptr<Player> currentFocusPlayer() const
	{
		return this->gameStateController->CurrentPlayer();
	}

	public: bool GameEnd = false;

	private: GameManager() = default;

	private: Shop shop;

		   // TODO: TeleportScroll detail panel
	private: void showCurrentDetailPanel()
	{
		if (this->gameStateController->CurrentState().HasState(EmGameState::TravelMap))
		{
			if (this->gameStateController->CurrentState().HasState(EmGameState::TeleportScroll_FreeTravelOnMap))
			{

			}
			else if (this->gameStateController->CurrentState().HasState(EmGameState::OpenBag))
			{
				this->uiManager->Scene_TravelMap->DetailConsole.SetInteractWithBag(
					std::make_shared<UI::Scene_TravelMap::InteractWithBag>(this->playerManager->GetItems())
				);
			}
			else if (this->gameStateController->CurrentState().HasState(EmGameState::Shop))
			{
				this->uiManager->Scene_TravelMap->DetailConsole.SetInteractWithShop(
					std::make_shared<UI::Scene_TravelMap::InteractWithShop>(this->shop)
				);
			}
			else if (this->gameStateController->CurrentState().HasState(EmGameState::InteractAndShowDetail))
			{
				this->uiManager->Scene_TravelMap->DetailConsole.SetInteractWithEnemy(
					std::make_shared<UI::Scene_TravelMap::InteractWithEnemy>(
						tempPrepareBattleEntities->Players,
						tempPrepareBattleEntities->Enemys
						)
				);
			}
			else
			{
				this->uiManager->Scene_TravelMap->DetailConsole.SetWhenTravelMap();
			}
		}
	}

	private: std::shared_ptr<BattleEntities> tempPrepareBattleEntities = nullptr;
	private: void interactWithEntity(std::shared_ptr<Player> player, std::vector<std::shared_ptr<Creature>> entities)
	{
		// show entities info
		auto entityType = entities[0]->Type();

		this->gameStateController->AddState(EmGameState::InteractAndShowDetail);
		// support battle
		std::vector<std::shared_ptr<Player>> battlePlayers{ player };
		std::vector<std::shared_ptr<Creature>> battleEnemys = entities;
		{
			// check arrond player
			for (const auto& arroundPlayer : players)
			{
				if (arroundPlayer == player)
					continue;
				if (!arroundPlayer->IsAlive())
					continue;

				if (entities[0]->Position().ManhattanDistance(arroundPlayer->Position()) > 3)
				{
					continue;
				}

				battlePlayers.push_back(arroundPlayer);
			}

			// check arrond enemy
			for (const auto& enemy : this->entityManager->GetEntities())
			{
				if (battleEnemys.size() >= 3)
					break;

				if (enemy->Type() != Entity::EmType::Enemy)
					continue;

				bool alreadySupport = LoDash::Any(battleEnemys, [&](const std::shared_ptr<Creature>& e)
					{
						return enemy == e;
					}
				);
				if (alreadySupport)
					continue;

				if (entities[0]->Position().ManhattanDistance(enemy->Position()) > 3)
				{
					continue;
				}

				battleEnemys.push_back(std::static_pointer_cast<Creature>(enemy));
			}
		}

		this->tempPrepareBattleEntities = std::make_shared<BattleEntities>(
			battlePlayers,
			battleEnemys
			);

		this->showCurrentDetailPanel();
	}

	private: void handleTravelMapKeyboardEvent(EmKeyBoard key)
	{
		if (this->currentFocusPlayer() == nullptr)
			return;

		auto nextPosition = this->currentFocusPlayer()->Position();
		Vector2D direction;
		switch (key)
		{
		case EmKeyBoard::UpArrow:
			direction = Vector2D::Up();
			break;
		case EmKeyBoard::DownArrow:
			direction = Vector2D::Down();
			break;
		case EmKeyBoard::LeftArrow:
			direction = Vector2D::Left();
			break;
		case EmKeyBoard::RightArrow:
			direction = Vector2D::Right();
			break;
		case EmKeyBoard::P:
			this->gameStateController->NextPlayerAction();
			return;
		case EmKeyBoard::I:
			this->gameStateController->AddState(EmGameState::OpenBag);
			this->showCurrentDetailPanel();
			return;
			//break;
		default:
			return;
		}

		if (this->gameStateController->UserTravelMapAction()->ActionPoint() <= 0)
		{
			return;
		}

		nextPosition += direction;

		if (auto optionTile = this->map->GetTile(nextPosition); !optionTile)
		{
			return;
		}
		else
		{
			auto entities = this->entityManager->PositionHasEntity(nextPosition);
			const auto& tile = optionTile.value();
			if (tile.Type == Tile::EmType::Shop)
			{
				this->gameStateController->AddState(EmGameState::Shop);
				if (auto walkable = this->currentFocusPlayer()->Walk(direction); walkable)
				{
					this->gameStateController->DoAction();
				}
				this->showCurrentDetailPanel();
			}
			else if (entities.size() != 0)
			{
				// There will not be different types of entities in the same position
				if (entities[0]->Type() == Entity::EmType::Player ||
					entities[0]->Type() == Entity::EmType::Tent)			// only player can interact with tent
				{
					if (auto walkable = this->currentFocusPlayer()->Walk(direction); walkable)
					{
						this->gameStateController->DoAction();
					}
				}
				else if (entities[0]->Type() == Entity::EmType::Enemy)
				{
					auto enemys = LoDash::Map(entities, [](const std::shared_ptr<Entity>& entity)
						{
							return std::static_pointer_cast<Creature>(entity);
						}
					);
					this->interactWithEntity(this->currentFocusPlayer(), enemys);
				}
				else if (entities[0]->Type() == Entity::EmType::WorldEvent)
				{
					auto worldEvent = std::static_pointer_cast<WorldEvent>(entities[0]);
					this->gameStateController->AddState(EmGameState::InteractWithWorldEvent);
					this->uiManager->Scene_TravelMap->DetailConsole.SetInteractWithWorldEvent(
						std::make_shared<UI::Scene_TravelMap::InteractWithWorldEvent>(this->gameStateController->CurrentPlayer(), worldEvent)
					);

					if (auto walkable = this->currentFocusPlayer()->Walk(direction); walkable)
					{
						this->gameStateController->DoAction();
					}
				}
			}
			else
			{
				if (auto walkable = this->currentFocusPlayer()->Walk(direction); walkable)
				{
					this->gameStateController->DoAction();
				}
			}
		}

		this->uiManager->Scene_TravelMap->MapCenterPosition = this->currentFocusPlayer()->Position();
	}
	private: void handleInteraceWithWorldEvent(EmKeyBoard key)
	{
		auto& panel = this->uiManager->Scene_TravelMap->DetailConsole;
		switch (key)
		{
		case EmKeyBoard::UpArrow:
			[[fallthrough]];
		case EmKeyBoard::RightArrow:
		{
			panel.ChangeInteractWithWorldEventFocusAmount(1);
		}
		break;
		case EmKeyBoard::DownArrow:
			[[fallthrough]];
		case EmKeyBoard::LeftArrow:
		{
			panel.ChangeInteractWithWorldEventFocusAmount(-1);
		}
		break;
		case EmKeyBoard::Enter:
		{
			if (panel.InteractWithWorldEvent->RollResult == -1)
			{
				panel.InteractWithWorldEvent->RollResult = panel.InteractWithWorldEvent->WorldEvent->Roll(
					this->currentFocusPlayer(),
					panel.InteractWithWorldEvent->FocusAmount
				);
				panel.InteractWithWorldEvent->WorldEvent->Happen(
					this->currentFocusPlayer(),
					panel.InteractWithWorldEvent->RollResult
				);

				this->gameStateController->CurrentPlayer()->UseFocus(panel.InteractWithWorldEvent->FocusAmount);
				this->entityManager->RemoveEntity(panel.InteractWithWorldEvent->WorldEvent);
			}
			else
			{
				this->gameStateController->RemoveState(EmGameState::InteractWithWorldEvent);

				this->showCurrentDetailPanel();
			}
		}
		break;
		default:
			return;
		}
	}

	private: Vector2D freeTravelMapPosition = Vector2D(0, 0);
	public: void StartFreeTravelMap(const Vector2D& start)
	{
		this->gameStateController->AddState(EmGameState::TeleportScroll_FreeTravelOnMap);
		this->freeTravelMapPosition = start;
	}
	private: void handleTeleportScroll_FreeTravelMapKeyboardEvent(EmKeyBoard key)
	{
		auto nextPosition = this->freeTravelMapPosition;
		Vector2D direction;
		switch (key)
		{
		case EmKeyBoard::UpArrow:
			direction = Vector2D::Up();
			break;
		case EmKeyBoard::DownArrow:
			direction = Vector2D::Down();
			break;
		case EmKeyBoard::LeftArrow:
			direction = Vector2D::Left();
			break;
		case EmKeyBoard::RightArrow:
			direction = Vector2D::Right();
			break;
		case EmKeyBoard::Enter:
		{
			// valid teleport 
			auto valid = LoDash::All(this->entityManager->GetEntities(), [&](const std::shared_ptr<Entity>& entity)
				{
					if (entity->Type() == Entity::EmType::Player)
						return true;
					if (entity->Type() == Entity::EmType::Tent)
						return true;

					if (entity->Position() == nextPosition)
					{
						return false;
					}
					return true;
				}
			);
			if (auto option = this->map->GetTile(nextPosition); !option.has_value())
			{
				valid = false;
			}
			else
			{
				auto tile = option.value();
				if (tile.Type == Tile::EmType::Wall ||
					tile.Type == Tile::EmType::Shop ||
					tile.Type == Tile::EmType::Water)
				{
					valid = false;
				}
			}
			if (!valid)
			{
				return;
			}

			this->gameStateController->RemoveState(EmGameState::TeleportScroll_FreeTravelOnMap);
			this->gameStateController->ChangeState(EmGameState::TravelMap);

			this->currentFocusPlayer()->SetPosition(nextPosition);

			this->showCurrentDetailPanel();
			return;
		}
		break;
		default:
			return;
		}

		nextPosition += direction;
		if (nextPosition.X < 0 || nextPosition.Y < 0)
			return;
		if (nextPosition.X >= this->map->GetWidth() || nextPosition.Y >= this->map->GetHeight())
			return;

		this->freeTravelMapPosition = nextPosition;

		this->uiManager->Scene_TravelMap->MapCenterPosition = nextPosition;
	}
	private: void handleInteractAndShowDetailKeyboardEvent(EmKeyBoard key)
	{
		const auto& battle = [&]()
		{
			this->battleManager = std::make_shared<BattleManager>(tempPrepareBattleEntities);
			this->battleManager->Event_OnBattleResult.Register([&](std::shared_ptr<IBattleInfo> result)
				{
					uiManager->Scene_Battle->BattleInfoConsole.SetBattleDetailConsole(
						std::make_shared<UI::Scene_Battle::BattleDetailConsole>(result)
					);
				}
			);
			this->battleManager->Event_WaitingCreatureSelectAction.Register([&](std::shared_ptr<Creature> creature)
				{
					switch (creature->Type())
					{
					case Entity::EmType::Player:
					{
						this->gameStateController->AddState(EmGameState::BattleSelectSkill);
						this->uiManager->Scene_Battle->BattleInfoConsole.SetPlayerSelectSkill(
							std::make_shared<UI::Scene_Battle::PlayerSelectSkill>(std::static_pointer_cast<Player>(creature))
						);
					}
					break;
					case Entity::EmType::Enemy:
					{
						this->gameStateController->AddState(EmGameState::BattleWaitingConfirm);

						auto aiAgent = std::make_shared<ai::AIAgent>(creature, this->battleManager);
						auto battleInfo = aiAgent->Behavior();
						this->battleManager->Battle(battleInfo);
					}
					break;
					default:
						break;
					}
				}
			);
			//this->battleManager->Event_WaitingPlayerSelectAction.Register([&](std::shared_ptr<Player> player)
			//	{
			//		this->uiManager->Scene_Battle->BattleInfoConsole.SetPlayerSelectSkill(
			//			std::make_shared<UI::Scene_Battle::PlayerSelectSkill>(player)
			//		);
			//	}
			//);

			this->gameStateController->ChangeState(EmGameState::Battle);
			this->uiManager->AwakeBattle(this->battleManager);
			this->showCurrentDetailPanel();

			this->gameStateController->DoAction();

			this->battleManager->Process();

		};
		const auto& backToTravelMap = [&]()
		{
			this->gameStateController->RemoveState(EmGameState::InteractAndShowDetail);
			this->showCurrentDetailPanel();
			this->uiManager->Scene_TravelMap->MapCenterPosition = this->currentFocusPlayer()->Position();
			this->tempPrepareBattleEntities = nullptr;

			this->gameStateController->DoAction();
		};
		switch (key)
		{
		case EmKeyBoard::UpArrow:
			this->uiManager->Scene_TravelMap->DetailConsole.ChangeInteractWithEnemySelectAction(-1);
			break;
		case EmKeyBoard::DownArrow:
			this->uiManager->Scene_TravelMap->DetailConsole.ChangeInteractWithEnemySelectAction(1);
			break;
		case EmKeyBoard::Enter:
			switch (this->uiManager->Scene_TravelMap->DetailConsole.InteractWithEnemy->SelectAction)
			{
			case UI::Scene_TravelMap::InteractWithEnemy::EmAction::Attack:
				battle();
				break;
			case UI::Scene_TravelMap::InteractWithEnemy::EmAction::Back:
				backToTravelMap();
				break;
			default:
				backToTravelMap();
				break;
			}
			break;
		case EmKeyBoard::BackSpace:
			backToTravelMap();
			break;
		default:
			break;
		}
	}
	private: void handleInteractShopKeyboardEvent(EmKeyBoard key)
	{
		const auto& backToTravelMap = [&]()
		{
			this->gameStateController->RemoveState(EmGameState::Shop);
			this->showCurrentDetailPanel();
			this->uiManager->Scene_TravelMap->MapCenterPosition = this->currentFocusPlayer()->Position();
		};

		switch (key)
		{
		case EmKeyBoard::UpArrow:
			this->uiManager->Scene_TravelMap->DetailConsole.ChangeInteractWithShopSelectIndex(-1);
			break;
		case EmKeyBoard::DownArrow:
			this->uiManager->Scene_TravelMap->DetailConsole.ChangeInteractWithShopSelectIndex(1);
			break;
		case EmKeyBoard::Enter:
		{
			if (auto option = this->uiManager->Scene_TravelMap->DetailConsole.GetInteractWithShopSelectedItem(); option.has_value())
			{
				const auto& [item, price] = option.value();

				this->playerManager->BuyItem(item->Type, price);
			}
			else
			{
				backToTravelMap();
			}
		}
		break;
		case EmKeyBoard::BackSpace:
			backToTravelMap();
			break;
		case EmKeyBoard::I:
			this->gameStateController->AddState(EmGameState::OpenBag);
			this->showCurrentDetailPanel();
			break;
		default:
			break;
		}
	}

	private: void playerUseEquipment(std::shared_ptr<Equipment> equipment)
	{
		auto currentPartEquipment = this->gameStateController->CurrentPlayer()->GetEquipment(equipment->PartType);

		if (currentPartEquipment != nullptr && currentPartEquipment->Type == equipment->Type)
		{
			this->playerManager->TakeOffItem(equipment->Type);
			this->gameStateController->CurrentPlayer()->TakeOff(equipment);
		}
		else if (this->playerManager->GetItems().at(equipment->Type) > 0)
		{
			if (currentPartEquipment != nullptr)
			{
				this->playerManager->TakeOffItem(currentPartEquipment->Type);
				this->gameStateController->CurrentPlayer()->TakeOff(currentPartEquipment);
			}
			this->playerManager->EquipItem(equipment->Type);
			this->gameStateController->CurrentPlayer()->Wear(equipment);
		}

		this->uiManager->Scene_TravelMap->DetailConsole.SetInteractWithBag(
			std::make_shared<UI::Scene_TravelMap::InteractWithBag>(this->playerManager->GetItems())
		);
	}
	private: void playerUseItem(std::shared_ptr<Item> item)
	{
		if (this->playerManager->GetItems().at(item->Type) <= 0)
		{
			return;
		}

		if (auto successfulUseItem = item->Use(this->gameStateController->CurrentPlayer()); !successfulUseItem)
		{
			return;
		}

		this->playerManager->RemoveItem(item->Type);

		this->uiManager->Scene_TravelMap->DetailConsole.SetInteractWithBag(
			std::make_shared<UI::Scene_TravelMap::InteractWithBag>(this->playerManager->GetItems())
		);
	}
	private: void handleInteractBagKeyboardEvent(EmKeyBoard key)
	{
		const auto& backToTravelMap = [&]()
		{
			this->gameStateController->RemoveState(EmGameState::OpenBag);
			this->showCurrentDetailPanel();
			this->uiManager->Scene_TravelMap->MapCenterPosition = this->currentFocusPlayer()->Position();
		};

		switch (key)
		{
		case EmKeyBoard::UpArrow:
			this->uiManager->Scene_TravelMap->DetailConsole.ChangeInteractWithBagSelectIndex(-1);
			break;
		case EmKeyBoard::DownArrow:
			this->uiManager->Scene_TravelMap->DetailConsole.ChangeInteractWithBagSelectIndex(1);
			break;
		case EmKeyBoard::Enter:
		{
			auto item = this->uiManager->Scene_TravelMap->DetailConsole.GetInteractWithBagSelectedItem();
			if (item == nullptr)
			{
				backToTravelMap();
				return;
			}

			if (Item::IsEquipment(item->Type))
			{
				this->playerUseEquipment(std::static_pointer_cast<Equipment>(item));
			}
			else if (Item::IsItem(item->Type))
			{
				this->playerUseItem(item);
			}
		}
		break;
		case EmKeyBoard::BackSpace:
			backToTravelMap();
			break;
		default:
			break;
		}
	}

	private: void handleBattleSelectSkillKeyboardEvent(EmKeyBoard key)
	{
		switch (key)
		{
		case EmKeyBoard::UpArrow:
			this->uiManager->Scene_Battle->BattleInfoConsole.ChangePlayerSelectSkillIndex(-1);
			break;
		case EmKeyBoard::DownArrow:
			this->uiManager->Scene_Battle->BattleInfoConsole.ChangePlayerSelectSkillIndex(1);
			break;
		case EmKeyBoard::I:
		{
			this->gameStateController->AddState(EmGameState::BattleSelectItem);
			this->gameStateController->RemoveState(EmGameState::BattleSelectSkill);

			this->uiManager->Scene_Battle->BattleInfoConsole.SetSelectItem(
				std::make_shared<UI::Scene_Battle::SelectItem>(this->playerManager->GetItems())
			);
		}
		break;
		case EmKeyBoard::Enter:
		{
			auto selectSkill = this->uiManager->Scene_Battle->BattleInfoConsole.PlayerSelectSkill->GetSelectSkill();
			if (selectSkill == nullptr)
			{
				return;
			}

			if (selectSkill->CurrentCoolDowning() > 0)
			{
				return;
			}

			this->gameStateController->AddState(EmGameState::BattleSelectTarget);
			this->gameStateController->RemoveState(EmGameState::BattleSelectSkill);
			this->uiManager->Scene_Battle->BattleInfoConsole.SetSelectTarget(
				std::make_shared<UI::Scene_Battle::SelectTarget>(
					this->tempPrepareBattleEntities,
					selectSkill,
					this->uiManager->Scene_Battle->BattleInfoConsole.PlayerSelectSkill->Player
					)
			);
		}
		break;
		default:
			break;
		}
	}
	private: void handleBattleSelectItemKeyboardEvent(EmKeyBoard key)
	{
		switch (key)
		{
		case EmKeyBoard::UpArrow:
		{
			this->uiManager->Scene_Battle->BattleInfoConsole.ChangeSelectItemIndex(-1);
		}
		break;
		case EmKeyBoard::DownArrow:
		{
			this->uiManager->Scene_Battle->BattleInfoConsole.ChangeSelectItemIndex(1);
		}
		break;
		case EmKeyBoard::Enter:
		{
			auto item = this->uiManager->Scene_Battle->BattleInfoConsole.GetSelectedItem();
			if (item == nullptr)
			{
				return;
			}

			if (auto successfulUseItem = item->Use(this->battleManager->processController->CurrentCreature()); !successfulUseItem)
			{
				return;
			}
			this->playerManager->RemoveItem(item->Type);

			this->gameStateController->RemoveState(EmGameState::BattleSelectItem);

			// process EmState::CreatureTurn
			this->battleManager->Process();
			this->battleManager->Process();
		}
		break;
		case EmKeyBoard::BackSpace:
		{
			this->gameStateController->AddState(EmGameState::BattleSelectSkill);
			this->gameStateController->RemoveState(EmGameState::BattleSelectItem);

			this->uiManager->Scene_Battle->BattleInfoConsole.SetPlayerSelectSkill(
				std::make_shared<UI::Scene_Battle::PlayerSelectSkill>(
					std::static_pointer_cast<Player>(this->battleManager->processController->CurrentCreature())
					)
			);
		}
		break;
		default:
			break;
		}
	}
	private: void handleBattleSelectTargetKeyboardEvent(EmKeyBoard key)
	{
		switch (key)
		{
		case EmKeyBoard::UpArrow:
			[[fallthrough]];
		case EmKeyBoard::LeftArrow:
			this->uiManager->Scene_Battle->BattleInfoConsole.ChangeSelectTargetIndex(-1);
			break;
		case EmKeyBoard::DownArrow:
			[[fallthrough]];
		case EmKeyBoard::RightArrow:
			this->uiManager->Scene_Battle->BattleInfoConsole.ChangeSelectTargetIndex(1);
			break;
		case EmKeyBoard::Enter:
		{
			this->gameStateController->AddState(EmGameState::BattleUseFocus);
			this->gameStateController->RemoveState(EmGameState::BattleSelectTarget);
			this->uiManager->Scene_Battle->BattleInfoConsole.SetUseFocus(
				std::make_shared<UI::Scene_Battle::UseFocus>(
					this->uiManager->Scene_Battle->BattleInfoConsole.PlayerSelectSkill->Player,
					this->uiManager->Scene_Battle->BattleInfoConsole.PlayerSelectSkill->GetSelectSkill()
					)
			);
		}
		break;
		case EmKeyBoard::BackSpace:
		{
			this->gameStateController->AddState(EmGameState::BattleSelectSkill);
			this->gameStateController->RemoveState(EmGameState::BattleSelectTarget);
			this->uiManager->Scene_Battle->BattleInfoConsole.SetPlayerSelectSkill(
				std::make_shared<UI::Scene_Battle::PlayerSelectSkill>(
					std::static_pointer_cast<Player>(this->battleManager->processController->CurrentCreature())
					)
			);
		}
		break;
		default:
			break;
		}
	}
	private: void handleBattleUseFocusKeyboardEvent(EmKeyBoard key)
	{
		switch (key)
		{
		case EmKeyBoard::UpArrow:
			[[fallthrough]];
		case EmKeyBoard::RightArrow:
			this->uiManager->Scene_Battle->BattleInfoConsole.ChangeUseFocusAmount(1);
			break;
		case EmKeyBoard::DownArrow:
			[[fallthrough]];
		case EmKeyBoard::LeftArrow:
			this->uiManager->Scene_Battle->BattleInfoConsole.ChangeUseFocusAmount(-1);
			break;
		case EmKeyBoard::Enter:
		{
			// handle battle
			auto battleInfo = this->uiManager->Scene_Battle->BattleInfoConsole.UseFocus->Skill->BattleInfo(
				this->uiManager->Scene_Battle->BattleInfoConsole.SelectTarget->GetTargets(),
				this->tempPrepareBattleEntities,
				this->uiManager->Scene_Battle->BattleInfoConsole.UseFocus->FocusAmount
			);

			this->gameStateController->AddState(EmGameState::BattleWaitingConfirm);
			this->gameStateController->RemoveState(EmGameState::BattleUseFocus);

			this->battleManager->Battle(std::static_pointer_cast<ActiveBattleInfo>(battleInfo));
		}
		break;
		case EmKeyBoard::BackSpace:
		{
			this->gameStateController->AddState(EmGameState::BattleSelectTarget);
			this->gameStateController->RemoveState(EmGameState::BattleUseFocus);
			this->uiManager->Scene_Battle->BattleInfoConsole.SetSelectTarget(
				std::make_shared<UI::Scene_Battle::SelectTarget>(
					this->tempPrepareBattleEntities,
					this->uiManager->Scene_Battle->BattleInfoConsole.PlayerSelectSkill->GetSelectSkill(),
					this->uiManager->Scene_Battle->BattleInfoConsole.PlayerSelectSkill->Player
					)
			);
		}
		break;
		default:
			break;
		}
	}
		   /// <summary>
		   ///		
		   /// </summary>
		   /// <returns>
		   ///		when game over return true
		   /// </returns>
	private: bool handleWhenAfterBattle()
	{
		// remove dead entitties
		for (const auto& entity : this->battleManager->battleEntities->Players)
		{
			if (!entity->IsAlive())
			{
				//auto equipments = entity->Equipments();
				//for (const auto& equipment : equipments)
				//{
				//	this->playerManager->TakeOffItem(equipment->Type);
				//	this->playerManager->RemoveItem(equipment->Type);
				//}

				this->entityManager->RemoveEntity(entity);
			}
		}
		for (const auto& entity : this->battleManager->battleEntities->Enemys)
		{
			if (!entity->IsAlive())
			{
				this->entityManager->RemoveEntity(entity);
			}
		}

		// check game over
		if (LoDash::All(this->players,
			[&](const std::shared_ptr<Player>& player) -> bool {
				return !player->IsAlive();
			}))
		{
			return true;
		}

			this->battleManager = nullptr;
			this->tempPrepareBattleEntities = nullptr;

			return false;
	}
	private: void handleBattleWaitingConfirmKeyboardEvent(EmKeyBoard key)
	{
		this->uiManager->Scene_Battle->BattleInfoConsole.NextDetailConsole();
		if (this->uiManager->Scene_Battle->BattleInfoConsole.BattleDetailConsole->WaitingAmount <= 0)
		{
			this->gameStateController->RemoveState(EmGameState::BattleWaitingConfirm);

			this->battleManager->Process();

			if (this->battleManager->processController->State() != Battle::ProcessController::EmState::BattleEnd)
				return;

			if (auto gameOver = this->handleWhenAfterBattle(); gameOver)
			{
				this->GameEnd = true;
				return;
			}

			if (!this->gameStateController->CurrentPlayer()->IsAlive())
			{
				this->gameStateController->NextPlayerAction();
			}

			this->gameStateController->ChangeState(EmGameState::TravelMap);
			this->uiManager->Scene_TravelMap->MapCenterPosition = this->currentFocusPlayer()->Position();
			this->uiManager->AwakeTravelMap();
			this->showCurrentDetailPanel();
		}
	}


	private: Attribute randomGeneratePlayerAttribute()
	{
		return Attribute{
			std::rand() % 15 + 30,
			3,
			std::rand() % 10 + 5,
			std::rand() % 10 + 5,
			std::rand() % 20,
			std::rand() % 20,
			std::rand() % 20 + 40,
			std::rand() % 25 + 30,
		};
	}
	private: void init()
	{
		std::vector<std::vector<Tile>> mapData;
		for (int h = 0; h < 50; h++)
		{
			std::vector<Tile> row;
			for (int w = 0; w < 140; w++)
			{
				row.push_back(Tile(Tile::EmType::Road));
			}

			mapData.push_back(row);
		}

		std::vector<Vector2D> walls
		{
			Vector2D(136, 47),
			Vector2D(136, 48),
			Vector2D(136, 49),
			Vector2D(137, 47),
			Vector2D(138, 47),
			Vector2D(139, 47),
		};
		for (const auto& pos : walls)
		{
			mapData[pos.Y][pos.X] = Tile(Tile::EmType::Wall);
		}

		mapData[45][133] = Tile(Tile::EmType::Shop);

		this->map = std::make_shared<Map>(mapData);

		this->players = std::vector<std::shared_ptr<Player>>{
			std::make_shared<Player>(
				"Player1",
				UI::Pixel{
					L'1',
					UI::PixelColor(Util::ForeGroundColor(EmColor::White))
				},
				Entity::EmType::Player,
				Vector2D(135, 47),
				Attribute({ 26, 6, 24, 6, 0, 12, 1, 90, }),
				this->map
			),
			std::make_shared<Player>(
				"Player2",
				UI::Pixel{
					L'2',
					UI::PixelColor(Util::ForeGroundColor(EmColor::White))
				},
				Entity::EmType::Player,
				Vector2D(50, 11),
				Attribute({ 26, 6, 24, 6, 0, 12, 80, 80, }),
				this->map
			),
			std::make_shared<Player>(
				"Player3",
				UI::Pixel{
					L'3',
					UI::PixelColor(Util::ForeGroundColor(EmColor::White))
				},
				Entity::EmType::Player,
				Vector2D(50, 12),
				//randomGeneratePlayerAttribute(),
				Attribute({ 33, 3, 13, 7, 16, 0, 53, 38, }),
				this->map
			),
		};

		std::vector<Shop::ItemAndPrice> items{
			// Misc
			std::make_pair(std::make_shared<item::Godsbeard>(), 5),
			std::make_pair(std::make_shared<item::GoldenRoot>(), 8),
			std::make_pair(std::make_shared<item::TeleportScroll>(), 15),
			std::make_pair(std::make_shared<item::Tent>(), 10),

			// Weapon
			std::make_pair(std::make_shared<item::WoodenSword>(), 10),
			std::make_pair(std::make_shared<item::Hammer>(), 10),
			std::make_pair(std::make_shared<item::GiantHammer>(), 10),
			std::make_pair(std::make_shared<item::MagicWand>(), 10),
			std::make_pair(std::make_shared<item::RitualSword>(), 999),

			// Armor
			std::make_pair(std::make_shared<item::PlateArmor>(), 10),
			std::make_pair(std::make_shared<item::LeatherArmor>(), 10),
			std::make_pair(std::make_shared<item::LaurelWreath>(), 10),
			std::make_pair(std::make_shared<item::Robe>(), 10),
			std::make_pair(std::make_shared<item::WoodenShield>(), 10),

			// Accessory
			std::make_pair(std::make_shared<item::HolyGrail>(), 10),
			std::make_pair(std::make_shared<item::Bracelet>(), 10),
			std::make_pair(std::make_shared<item::Shoes>(), 10),
		};
		for (const auto& [item, price] : items)
		{
			this->shop.AddItem(item, price);
		}
	}
	public: void Init()
	{
		this->init();

		{
			this->playerManager->Init(this->map);
		}
		{
			this->entityManager->Init();
			for (auto& player : this->players)
			{
				player->SetSkills({
					std::make_shared<NormalAttack>(player, 1),
					std::make_shared<Escape>(player)
					}
				);
				this->entityManager->AddEntity(player);
			}
			std::vector<std::shared_ptr<Creature>> enemy
			{
				std::make_shared<Creature>(
					"Enemy1",
					UI::Pixel{
						L'E',
						UI::PixelColor(Util::ForeGroundColor(EmColor::Red))
					},
					Entity::EmType::Enemy,
					Vector2D(137, 49),
					Attribute({ 200, 0, 100, 0, 0, 0, 100, 50, })
				),

				std::make_shared<Creature>(
					"Enemy2",
					UI::Pixel{
						L'E',
						UI::PixelColor(Util::ForeGroundColor(EmColor::Red))
					},
					Entity::EmType::Enemy,
					Vector2D(50, 10),
					Attribute({ 100, 0, 10, 0, 0, 0, 100, 50, })
				),
				std::make_shared<Creature>(
					"Enemy3",
					UI::Pixel{
						L'E',
						UI::PixelColor(Util::ForeGroundColor(EmColor::Red))
					},
					Entity::EmType::Enemy,
					Vector2D(50, 9),
					Attribute({ 20, 0, 10, 0, 0, 0, 100, 50, })
				),
				std::make_shared<Creature>(
					"Enemy4",
					UI::Pixel{
						L'E',
						UI::PixelColor(Util::ForeGroundColor(EmColor::Red))
					},
					Entity::EmType::Enemy,
					Vector2D(50, 8),
					Attribute({ 20, 0, 10, 0, 0, 0, 100, 50, })
				),

				std::make_shared<Creature>(
					"Slime",
					UI::Pixel{
						L'E',
						UI::PixelColor(Util::ForeGroundColor(EmColor::Red))
					},
					Entity::EmType::Enemy,
					Vector2D(45, 8),
					Attribute({ 50, 0, 10, 0, 0, 0, 100, 50, })
				),
			};
			for (auto& e : enemy)
			{
				e->SetSkills({ std::make_shared<NormalAttack>(e, 1) });
				this->entityManager->AddEntity(e);
			}
			enemy[4]->Wear(std::make_shared<item::DirtyStuff>());


			this->entityManager->AddEntity(std::make_shared<WorldEvent>(Vector2D(138, 48)));
		}
		{
			this->uiManager->Init(this->map, std::vector<std::shared_ptr<Player>>{this->players});
		}
		{
			this->gameStateController->Init(this->players);

			this->gameStateController->Event_PlayerAction.Register([&](FTK::UserTravelMapAction userAction)
				{
					this->uiManager->Scene_TravelMap->CleanFocusPanel();

					this->uiManager->Scene_TravelMap->MapCenterPosition = this->currentFocusPlayer()->Position();
					this->uiManager->Scene_TravelMap->DetailConsole.SetWhenTravelMap();
				}
			);
		}
		{
			this->userEvent->Init();
			this->userEvent->Event_KeyBoard.Register([&](EmKeyBoard key)
				{
					const auto& gameState = this->gameStateController->CurrentState();
					if (gameState.HasState(EmGameState::TravelMap))
					{
						if (gameState.HasState(EmGameState::InteractAndShowDetail))
						{
							this->handleInteractAndShowDetailKeyboardEvent(key);
						}
						else if (gameState.HasState(EmGameState::InteractWithWorldEvent))
						{
							this->handleInteraceWithWorldEvent(key);
						}
						else if (gameState.HasState(EmGameState::TeleportScroll_FreeTravelOnMap))
						{
							this->handleTeleportScroll_FreeTravelMapKeyboardEvent(key);
						}
						else if (gameState.HasState(EmGameState::OpenBag))
						{
							this->handleInteractBagKeyboardEvent(key);
						}
						else if (gameState.HasState(EmGameState::Shop))
						{
							this->handleInteractShopKeyboardEvent(key);
						}
						else
						{
							this->handleTravelMapKeyboardEvent(key);
						}
					}
					else if (gameState.HasState(EmGameState::Battle))
					{
						if (gameState.HasState(EmGameState::BattleSelectSkill))
						{
							this->handleBattleSelectSkillKeyboardEvent(key);
						}
						else if (gameState.HasState(EmGameState::BattleSelectItem))
						{
							this->handleBattleSelectItemKeyboardEvent(key);
						}
						else if (gameState.HasState(EmGameState::BattleSelectTarget))
						{
							this->handleBattleSelectTargetKeyboardEvent(key);
						}
						else if (gameState.HasState(EmGameState::BattleUseFocus))
						{
							this->handleBattleUseFocusKeyboardEvent(key);
						}
						else if (gameState.HasState(EmGameState::BattleWaitingConfirm))
						{
							this->handleBattleWaitingConfirmKeyboardEvent(key);
						}
					}
				}
			);
		}

		this->gameStateController->NextTurn();

		this->showCurrentDetailPanel();
	}
	};
}
#endif // !GameManager_H
