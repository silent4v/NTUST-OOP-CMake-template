#include "Scene_TravelMap.h"

#include "../../Manager/EntityManager.hpp"
#include "../../Manager/PlayerManager.hpp"
#include "../../Manager/GameStateController.hpp"
#include "../../ConsoleRender.hpp"

namespace FTK
{
	namespace UI
	{
		namespace
		{
			const auto& gameStateController = GameStateController::Instance();
		}

		void Scene_TravelMap::renderMap()
		{
			//ConsoleRender::Instance().DrawEdge(this->panelMap.Position.X, this->panelMap.Position.Y, this->panelMap.Width, this->panelMap.Height);
			const auto& tiles = this->map->GetTiles();
			const int width = (this->Panel_MAP_WIDTH - 1) / 2;
			const int height = (this->Panel_MAP_HEIGHT - 1) / 2;

			Vector2D panelCenter(
				(this->Panel_MAP_WIDTH - 1) / 2 + 1,
				(this->Panel_MAP_HEIGHT - 1) / 2 + 1
			);
			const auto& renderTile = [&](const Vector2D& offset)
				{
					auto panelPosition = panelCenter + offset;
					if (panelPosition.X <= 1 || panelPosition.X >= this->Panel_MAP_WIDTH || panelPosition.Y <= 1 || panelPosition.Y >= this->Panel_MAP_HEIGHT)
						return;

					auto wordPosition = this->MapCenterPosition + offset;
					if (wordPosition.X < 0 || wordPosition.X >= tiles[0].size() || wordPosition.Y < 0 || wordPosition.Y >= tiles.size())
					{
						this->panelMap.DrawPixel(panelPosition, Pixel(L' ', 0x00));

						return;
					}

					const auto& tile = tiles[wordPosition.Y][wordPosition.X];
					this->panelMap.DrawPixel(panelPosition, Pixel(
						TileToCharacter(tile),
						TileToForeGroundColor(tile), TileToBackGroundColor(tile)
					));
				};

			for (int h = 0; h < height; h++)
			{
				for (int w = 0; w < width; w++)
				{
					renderTile(Vector2D(w, h));
					renderTile(Vector2D(w, -h));
					renderTile(Vector2D(-w, h));
					renderTile(Vector2D(-w, -h));
				}
			}

			const auto& entities = EntityManager::Instance().GetEntities();
			for (const auto& entity : entities)
			{
				if (entity->Dead)
					continue;

				auto offset = entity->Position() - this->MapCenterPosition;
				auto panelPosition = panelCenter + offset;
				if (panelPosition.X <= 1 || panelPosition.X >= this->Panel_MAP_WIDTH ||
					panelPosition.Y <= 1 || panelPosition.Y >= this->Panel_MAP_HEIGHT)
					continue;

				auto renderPixel = entity->RenderPixel();
				if (auto option = this->map->GetTile(entity->Position()); option.has_value())
				{
					const auto& tile = option.value();
					renderPixel.Color.BackGroundColor = TileToBackGroundColor(tile);
				}

				this->panelMap.DrawPixel(
					panelCenter + entity->Position() - this->MapCenterPosition,
					renderPixel
				);
			}

			if (gameStateController.CurrentState().HasState(EmGameState::TeleportScroll_FreeTravelOnMap))
			{
				auto tile = tiles[this->MapCenterPosition.Y][this->MapCenterPosition.X];
				this->panelMap.DrawPixel(panelCenter, Pixel(
					L'X',
					Util::ForeGroundColor(EmColor::White),
					TileToBackGroundColor(tile)
				));
			}
		}

#pragma region DetailConsole

		DrawInfo drawDivider(const std::string title, const Vector2D& offset = Vector2D(0, 0))
		{
			return DrawInfo{
				std::string((Scene_TravelMap::Panel_DetailConsole_WIDTH - title.size() - 4) / 2, '_') +
					title +
					std::string((Scene_TravelMap::Panel_DetailConsole_WIDTH - title.size() - 4) / 2, '_'),
				PixelColor(EmColor::White, EmColor::Black),
				offset
			};
		}

		void drawHelpInfo2DVec(
			std::vector<DrawInfo>& result,
			const std::vector<std::vector<DrawInfo>>& helpInfo2DVec
		) {
			const int rowLengthLimit = Scene_TravelMap::Panel_DetailConsole_HEIGHT - 1;

			for (int i = 0; i < helpInfo2DVec.size(); i++)
			{
				auto& helpInfos = helpInfo2DVec[i];
				int len = 0;
				for (auto helpInfo : helpInfos)
				{
					helpInfo.Offset.X = len + 2;
					len += helpInfo.Content.size();
					helpInfo.Offset.Y = rowLengthLimit - helpInfo2DVec.size() + i;

					result.push_back(helpInfo);
				}
			}
		}

		std::vector<DrawInfo> Scene_TravelMap::InteractWithEnemy::ToString() const
		{
			std::vector<DrawInfo> result;

			int columnLength = Scene_TravelMap::Panel_DetailConsole_WIDTH;

			Vector2D offset = Vector2D{ 1, 2 };

			// ! In one interaction, at most three entities can appear at the same time
			for (int i = 0; i < 3; i++)
			{
				if (i < this->players.size())
				{
					result.push_back(DrawInfo{
						this->players[i]->Name(),
						PixelColor(EmColor::White, EmColor::Black),
						offset + Vector2D{0, i}
						}
					);
				}
				if (i < this->enemys.size())
				{
					std::string enemyName = this->enemys[i]->Name();
					result.push_back(DrawInfo{
						enemyName,
						PixelColor(EmColor::Red, EmColor::Black),
						Vector2D{int(columnLength - enemyName.size()) - offset.X, i + offset.Y}
						}
					);
				}
			}
			offset.Y += 4;

			// show action
			//	1. Attack
			//  TODO: Ambush
			//	2. Back
			std::string actionName[ACTION_COUNT] = { "Attack", "Back" };
			for (int i = 0; i < 2; i++)
			{
				if (this->SelectAction == EmAction(i))
				{
					result.push_back(DrawInfo{
						"->",
						PixelColor(EmColor::White, EmColor::Black),
						offset + Vector2D{0, i}
						}
					);
				}
				else
				{
					result.push_back(DrawInfo{
						"  ",
						PixelColor(EmColor::White, EmColor::Black),
						offset + Vector2D{0, i}
						}
					);
				}

				result.push_back(DrawInfo{
					actionName[i],
					PixelColor(EmColor::White, EmColor::Black),
					offset + Vector2D{3, i}
					}
				);
			}


			// help info
			std::vector<std::vector<DrawInfo>> helpInfo2DVec{
				{
					drawDivider("Helper")
				},
				{
					DrawInfo{
						"Arrow Key (Up/Down): Choose Action",
						PixelColor(EmColor::White, EmColor::Black),
					},
				},
				{
					DrawInfo{
						"Enter: Select Action",
						PixelColor(EmColor::White, EmColor::Black),
					},
				},
			};
			drawHelpInfo2DVec(result, helpInfo2DVec);

			return result;

			return result;
		}

		std::vector<DrawInfo> Scene_TravelMap::InteractWithShop::ToString() const
		{
			std::vector<DrawInfo> result;

			auto itemList = LoDash::Values(this->Shop.ItemMappingPrice);

			const int scollLength = 8;
			int page = this->SelectIndex / scollLength;
			int itemIndexOffset = this->SelectIndex % scollLength;
			auto startIndex = page * scollLength;

			result.push_back(DrawInfo{
				"Shop  (Page:" + std::to_string(page + 1) + "/" + std::to_string(itemList.size() / scollLength + 1) + ")",
				PixelColor(EmColor::White, EmColor::Black),
				Vector2D{2, 1}
				}
			);
			Vector2D offset = Vector2D{ 2, 2 };
			for (int i = startIndex; i < startIndex + 8; i++)
			{
				if (i >= itemList.size())
				{
					break;
				}

				if (this->SelectIndex == i)
				{
					result.push_back(DrawInfo{
						"->",
						PixelColor(EmColor::White, EmColor::Black),
						offset + Vector2D{0, i - startIndex}
						}
					);
				}
				else
				{
					result.push_back(DrawInfo{
						"  ",
						PixelColor(EmColor::White, EmColor::Black),
						offset + Vector2D{0, i - startIndex}
						}
					);
				}

				const auto& [item, price] = itemList[i];
				result.push_back(DrawInfo{
					item->Name + ": " + std::to_string(price) + "$",
					PixelColor(
						PlayerManager::Instance().GetMoney() >= price ? EmColor::White : EmColor::Red,
						EmColor::Black
					),
					offset + Vector2D{3, i - startIndex}
					}
				);
			}

			offset.Y += 8 + 2;
			if (0 <= this->SelectIndex && this->SelectIndex < itemList.size())
			{
				const auto& [item, _] = itemList[this->SelectIndex];
				std::string description = item->Description;
				std::vector<std::string> descriptionLines;
				std::string temp = "";
				for (const auto& c : description)
				{
					temp += c;
					if (temp.size() >= Scene_TravelMap::Panel_DetailConsole_WIDTH - 2)
					{
						descriptionLines.push_back(temp);
						temp = "";
					}
				}
				if (temp.size() > 0)
				{
					descriptionLines.push_back(temp);
				}

				for (int i = 0; i < descriptionLines.size(); i++)
				{
					result.push_back(DrawInfo{
						descriptionLines[i],
						PixelColor(EmColor::White, EmColor::Black),
						offset + Vector2D{0, i}
						}
					);
				}
			}

			// help info
			std::vector<std::vector<DrawInfo>> helpInfo2DVec{
				{
					DrawInfo{
						"Money: " + std::to_string(PlayerManager::Instance().GetMoney()) + "$",
						PixelColor(EmColor::White, EmColor::Black),
					}
				},
				{
					drawDivider("Helper")
				},
				{
					DrawInfo{
						"Arrow Key (Up/Down): Choose item",
						PixelColor(EmColor::White, EmColor::Black),
					},
				},
				{
					DrawInfo{
						"Enter: Buying item",
						PixelColor(EmColor::White, EmColor::Black),
					},
				},
				{
					DrawInfo{
						"Backspace: Back",
						PixelColor(EmColor::White, EmColor::Black),
					},
				},
			};
			drawHelpInfo2DVec(result, helpInfo2DVec);

			return result;
		}

		std::vector<DrawInfo> Scene_TravelMap::InteractWithBag::ToString() const
		{
			std::vector<DrawInfo> result;

			const int scollLength = 8;
			int page = this->SelectIndex / scollLength;
			int itemIndexOffset = this->SelectIndex % scollLength;
			auto startIndex = page * scollLength;

			result.push_back(DrawInfo{
				"Bag (Page:" + std::to_string(page + 1) + "/" + std::to_string(itemList.size() / scollLength + 1) + ")",
				PixelColor(EmColor::White, EmColor::Black),
				Vector2D{2, 1}
				}
			);

			Vector2D offset = Vector2D{ 2, 2 };
			if (PlayerManager::Instance().GetItems().size() == 0)
			{
				result.push_back(DrawInfo{
					"None item",
					PixelColor(EmColor::White, EmColor::Black),
					offset
					}
				);
			}
			else
			{
				for (int i = startIndex; i < startIndex + 8; i++)
				{
					if (i >= itemList.size())
					{
						break;
					}

					if (this->SelectIndex == i)
					{
						result.push_back(DrawInfo{
							"->",
							PixelColor(EmColor::White, EmColor::Black),
							offset + Vector2D{0, i - startIndex}
							}
						);
					}

					auto item = itemList[i];
					std::string itemInfo =
						itemList[i]->Name + ": " +
						std::to_string(this->BagItems.at(itemList[i]->Type));

					if (Item::IsEquipment(item->Type))
					{
						const auto& alreadyEquippedItems = PlayerManager::Instance().AlreadyEquippedItems();
						if (auto itor = alreadyEquippedItems.find(item->Type); itor != alreadyEquippedItems.end())
						{
							const auto& [item, amount] = *itor;
							itemInfo += ", E*" + std::to_string(amount);
						}
					}
					result.push_back(DrawInfo{
						itemInfo,
						PixelColor(EmColor::White, EmColor::Black),
						offset + Vector2D{3, i - startIndex}
						}
					);
				}
				// show item description
				offset.Y += 8 + 1;
				result.push_back(drawDivider("Item Description", offset));

				// show description
				offset.Y += 1;
				if (0 <= this->SelectIndex && this->SelectIndex < itemList.size())
				{
					std::string description = itemList[this->SelectIndex]->Description;
					std::vector<std::string> descriptionLines;
					std::string temp = "";
					for (const auto& c : description)
					{
						temp += c;
						if (temp.size() >= Scene_TravelMap::Panel_DetailConsole_WIDTH - 2)
						{
							descriptionLines.push_back(temp);
							temp = "";
						}
					}
					if (temp.size() > 0)
					{
						descriptionLines.push_back(temp);
					}

					for (int i = 0; i < descriptionLines.size(); i++)
					{
						result.push_back(DrawInfo{
							descriptionLines[i],
							PixelColor(EmColor::White, EmColor::Black),
							offset + Vector2D{0, i}
							}
						);
					}
				}
			}

			// help info
			std::vector<std::vector<DrawInfo>> helpInfo2DVec{
				{
					DrawInfo{
						"Money: " + std::to_string(PlayerManager::Instance().GetMoney()) + "$",
						PixelColor(EmColor::White, EmColor::Black),
					}
				},
				{
					drawDivider("Helper")
				},
				{
					DrawInfo{
						"Arrow Key (Up/Down): Choose item",
						PixelColor(EmColor::White, EmColor::Black),
					},
				},
				{
					DrawInfo{
						"Enter: Using item",
						PixelColor(EmColor::White, EmColor::Black),
					},
				},
				{
					DrawInfo{
						"Backspace: Back",
						PixelColor(EmColor::White, EmColor::Black),
					},
				},
			};
			drawHelpInfo2DVec(result, helpInfo2DVec);

			return result;
		}

		std::vector<DrawInfo> Scene_TravelMap::WhenTravelMap::ToString() const
		{
			std::vector<DrawInfo> result;

			auto userAction = GameStateController::Instance().UserTravelMapAction();

			if (userAction == nullptr)
			{
				return result;
			}

			Vector2D offset = Vector2D{ 2, 1 };
			result.push_back(DrawInfo{
				"Turn: " + std::to_string(GameStateController::Instance().Turn()),
				PixelColor(EmColor::White, EmColor::Black),
				offset
				}
			);

			offset.Y += 1;
			std::string playerName = "PlayerName: " + userAction->Player->Name();
			result.push_back(DrawInfo{
				playerName,
				PixelColor(EmColor::White, EmColor::Black),
				offset
				}
			);

			offset.Y += 1;
			std::string actionPointString = "Action Point: ";
			result.push_back(DrawInfo{
				actionPointString,
				PixelColor(EmColor::White, EmColor::Black),
				offset
				}
			);
			int failActionPoint = userAction->FailActionPoint();
			for (int i = 0; i < failActionPoint; i++)
			{
				result.push_back(DrawInfo{
					"*",
					PixelColor(EmColor::Red, EmColor::Black),
					offset + Vector2D{int(actionPointString.size() + i), 0}
					}
				);
			}
			int maxActionPoint = userAction->MaxActionPoint();
			for (int i = 0; i < maxActionPoint; i++)
			{
				result.push_back(DrawInfo{
					"*",
					PixelColor(
						i < userAction->ActionPoint() ? EmColor::Yellow : EmColor::DarkGray,
						EmColor::Black
					),
					offset + Vector2D{int(actionPointString.size() + failActionPoint + i), 0}
					}
				);
			}

			// help info
			std::vector<std::vector<DrawInfo>> helpInfo2DVec{
				{
					drawDivider("Helper")
				},
				{
					DrawInfo{
						"Arrow Key: Move",
						PixelColor(EmColor::White, EmColor::Black),
					},
				},
				{
					DrawInfo{
						"\'P\' Key: End Turn",
						PixelColor(EmColor::White, EmColor::Black),
					},
				},
				{
					DrawInfo{
						"\'I\' Key: Open Bag",
						PixelColor(EmColor::White, EmColor::Black),
					},
				},
				{
					DrawInfo{
						Scene_TravelMap::TileToCharacterString(Tile::EmType::Wall),
						PixelColor(
							Scene_TravelMap::TileToForeGroundColor(Tile::EmType::Wall),
							Scene_TravelMap::TileToBackGroundColor(Tile::EmType::Wall)
						),
					},
					DrawInfo{
						":Wall, ",
						PixelColor(EmColor::White, EmColor::Black),
					},
					DrawInfo{
						Scene_TravelMap::TileToCharacterString(Tile::EmType::Road),
						PixelColor(
							Scene_TravelMap::TileToForeGroundColor(Tile::EmType::Road),
							Scene_TravelMap::TileToBackGroundColor(Tile::EmType::Road)
						),
					},
					DrawInfo{
						":Road, ",
						PixelColor(EmColor::White, EmColor::Black),
					},
					DrawInfo{
						Scene_TravelMap::TileToCharacterString(Tile::EmType::Shop),
						PixelColor(
							Scene_TravelMap::TileToForeGroundColor(Tile::EmType::Shop),
							Scene_TravelMap::TileToBackGroundColor(Tile::EmType::Shop)
						),
					},
					DrawInfo{
						":Shop",
						PixelColor(EmColor::White, EmColor::Black),
					},
				},
			};
			drawHelpInfo2DVec(result, helpInfo2DVec);

			return result;
		}

		std::vector<DrawInfo> Scene_TravelMap::InteractWithWorldEvent::ToString() const
		{
			std::vector<DrawInfo> drawInfos;

			if (this->RollResult == -1)
			{
				Vector2D offset = Vector2D{ 2, 1 };
				drawInfos.push_back(DrawInfo{
					"Random Event:",
					PixelColor(EmColor::White, EmColor::Black),
					offset
					}
				);

				// probability
				offset.Y += 1;
				drawInfos.push_back(DrawInfo{
					"Probability each roll: " + std::to_string(this->WorldEvent->Probability(
						std::static_pointer_cast<FTK::Player>(this->Player)
					)),
					PixelColor(EmColor::White, EmColor::Black),
					offset
					}
				);

				offset.Y += 1;
				for (int i = 0; i <= this->WorldEvent->RollAmount(); i++)
				{
					drawInfos.push_back(DrawInfo{
						"Roll (" + std::to_string(i) + "):" + this->WorldEvent->Description(i),
						PixelColor(EmColor::White, EmColor::Black),
						offset + Vector2D{0, i}
						}
					);
				}

				// use focus
				offset.Y += this->WorldEvent->RollAmount() + 1;
				int maxFocus = this->Player->ModifyAttribute().Focus;
				int remainFocus = this->Player->CurrentAttribute().Focus;
				{
					std::string focusTitle = "UseFocus: ";

					drawInfos.push_back(DrawInfo{
						focusTitle,
						PixelColor(EmColor::White, EmColor::Black),
						offset
						}
					);

					for (int i = 0; i < maxFocus; i++)
					{
						if (i < remainFocus)
						{
							drawInfos.push_back(DrawInfo{
								"*",
								PixelColor(i < this->FocusAmount ? EmColor::Yellow : EmColor::DarkGray, EmColor::Black),
								offset + Vector2D(focusTitle.size() + i, 0)
								}
							);
						}
						else
						{
							drawInfos.push_back(DrawInfo{
								"*",
								PixelColor(EmColor::Red, EmColor::Black),
								offset + Vector2D(focusTitle.size() + i, 0)
								}
							);
						}
					}
				}
			}
			else
			{
				Vector2D offset = Vector2D{ 2, 1 };
				drawInfos.push_back(DrawInfo{
								"Random Event:",
								PixelColor(EmColor::White, EmColor::Black),
								offset
					}
				);

				// show result
				offset.Y += 1;
				drawInfos.push_back(DrawInfo{
					"Roll: " + std::to_string(this->RollResult),
					PixelColor(EmColor::White, EmColor::Black),
					offset
					}
				);
				// show description
				offset.Y += 1;
				std::string description = this->WorldEvent->Description(this->RollResult);
				drawInfos.push_back(DrawInfo{
					description,
					PixelColor(EmColor::White, EmColor::Black),
					offset
					}
				);

				// input continue to next
				offset.Y += 1;
				drawInfos.push_back(DrawInfo{
					"Press Enter to continue",
					PixelColor(EmColor::White, EmColor::Black),
					offset
					}
				);
			}

			return drawInfos;
		}

		void Scene_TravelMap::renderDetailConsole()
		{
			this->panelDetailConsole.ClearPanel();

			auto contents = this->DetailConsole.ToString();
			if (contents.size() == 0)
			{
				return;
			}

			this->panelDetailConsole.DrawPanel(contents, false);

			if (gameStateController.CurrentState().HasState(EmGameState::OpenBag) ||
				gameStateController.CurrentState().HasState(EmGameState::InteractAndShowDetail) ||
				gameStateController.CurrentState().HasState(EmGameState::Shop))
			{
				this->CleanFocusPanel();
				this->focusPanel(this->panelDetailConsole);
			}
		}

#pragma endregion

		void Scene_TravelMap::renderPlayerInfos()
		{
			for (int i = 0; i < 3; i++)
			{
				if (i >= this->players.size())
				{
					this->panelPlayerInfos[i].DrawPanel("");
					continue;
				}

				const auto& player = this->players[i];
				auto& panel = this->panelPlayerInfos[i];
				this->drawCreatureInfo(player, panel,
					PixelColor(
						player->IsAlive() ? EmColor::White : EmColor::DarkGray,
						EmColor::Black
					)
				);
			}

			if (gameStateController.CurrentState().HasState(EmGameState::TravelMap) ||
				gameStateController.CurrentState().HasState(EmGameState::Shop) ||
				gameStateController.CurrentState().HasState(EmGameState::InteractAndShowDetail))
			{
				if (gameStateController.CurrentPlayer() != nullptr)
				{
					this->focusPanel(this->panelPlayerInfos[
						gameStateController.CurrentPlayerIndex()
					]);
				}
			}
		}

		void Scene_TravelMap::Awake()
		{
			this->panelMap.DrawPanel("");
			this->panelDetailConsole.DrawPanel("");
			for (int i = 0; i < 3; i++)
			{
				this->panelPlayerInfos[i].DrawPanel("");
			}
			this->DetailConsole.Reset();
		}

		void Scene_TravelMap::Render()
		{
			renderMap();
			renderDetailConsole();
			renderPlayerInfos();
		}
	}
}