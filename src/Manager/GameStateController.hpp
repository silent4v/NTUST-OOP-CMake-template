#ifndef GameStateController_H

#define GameStateController_H

#include <memory>

#include "../Util/Event.hpp"
#include "../Util/Singleton.hpp"
#include "PlayerManager.hpp"
#include "../Define/GameState.h"
#include "../Util/RandomSample.hpp"

namespace FTK
{
	class UserTravelMapAction
	{
	public: const std::shared_ptr<const Player> Player;
	private: int maxActionPoint = 0;
	public: inline int MaxActionPoint() const { return this->maxActionPoint; }
	private: int actionPoint = 0;
	public: inline int ActionPoint() const { return this->actionPoint; }
	public: inline void SetAction(int actionPoint)
	{
		this->actionPoint = actionPoint;
	}
	private: int failActionPoint = 0;
	public: inline int FailActionPoint() const { return this->failActionPoint; }

	public: UserTravelMapAction(std::shared_ptr<FTK::Player> player) :
		Player(player)
	{
		auto sucessful = player->GetActionPointProbability();
		std::vector<double> probabilities{
			1 - sucessful,
			sucessful,
		};

		auto probabilityMaxActionPoint = player->GetActionPoint();
		this->actionPoint = player->EnsureSuccessfulActionPoint();
		for (int i = player->EnsureSuccessfulActionPoint(); i < probabilityMaxActionPoint; i++)
		{
			int isSucessful = Util::RandomSample(probabilities, std::rand(), 2);
			if (isSucessful == 1)
			{
				this->actionPoint++;
			}
			else
			{
				this->failActionPoint++;
			}
		}
		this->maxActionPoint = this->actionPoint;
	}
	};

	class GameStateController : public Util::Singleton<GameStateController>
	{
	private: friend class Util::Singleton<GameStateController>;

	private: int turn = 0;
	public: inline int Turn() const { return turn; }

	private: std::vector<std::shared_ptr<Player>> players;
	public: inline const std::vector<std::shared_ptr<Player>>& Players() const { return players; }
	private: int currentPlayerIndex = 0;
	public: inline int CurrentPlayerIndex() const { return this->currentPlayerIndex; }
	public: inline std::shared_ptr<Player> CurrentPlayer() const { return this->players[currentPlayerIndex]; }
	public: inline void SetCurrentPlayer(int index)
	{
		this->currentPlayerIndex = index;
	}

		  /// <summary>
		  ///	Event when change player action
		  ///	Parameter:
		  ///		- UserTravelMapAction: current player action
		  /// </summary>
	public: Util::Event<void, FTK::UserTravelMapAction> Event_PlayerAction;
		  /// <summary>
		  ///	invoke when player end action, only alive player
		  /// </summary>
	public: Util::Event<void, std::shared_ptr<Player>> Event_PlayerEndAction;
	public: Util::Event<void> Event_NextTurn;

	private: GameState currentState = GameState(EmGameState::TravelMap);
	public: inline GameState CurrentState() const { return this->currentState; }
	public: inline void ChangeState(EmGameState newState)
	{
		this->currentState.ChangeState(newState);
	}
	public: inline void RemoveState(EmGameState state)
	{
		this->currentState.RemoveState(state);
	}
	public: inline void AddState(EmGameState state)
	{
		this->currentState.AddState(state);
	}

	private: std::shared_ptr<UserTravelMapAction> userTravelMapAction = nullptr;
	public: inline const std::shared_ptr<const UserTravelMapAction> UserTravelMapAction() const { return userTravelMapAction; }
	public: void NextTurn()
	{
		this->turn++;
		for (int nextPlayerIndex = 0; nextPlayerIndex < this->players.size(); nextPlayerIndex++)
		{
			if (this->players[nextPlayerIndex]->IsAlive())
			{
				this->currentPlayerIndex = nextPlayerIndex;
				break;
			}
		}
		this->userTravelMapAction = std::make_shared<FTK::UserTravelMapAction>(
			this->CurrentPlayer()
		);
		this->Event_NextTurn.Invoke();
		this->Event_PlayerAction.Invoke(*this->userTravelMapAction);
	}
	public: void NextPlayerAction()
	{
		if (this->CurrentPlayer()->IsAlive())
		{
			this->Event_PlayerEndAction.Invoke(this->CurrentPlayer());
		}

		if (this->currentPlayerIndex + 1 >= this->players.size())
		{
			this->NextTurn();
			return;
		}

		for (int i = 1; i <= this->players.size(); i++)
		{
			int index = (this->currentPlayerIndex + i) % this->players.size();
			if (this->players[index]->IsAlive())
			{
				this->currentPlayerIndex = index;
				break;
			}
		}

		this->userTravelMapAction = std::make_shared<FTK::UserTravelMapAction>(
			this->CurrentPlayer()
		);
		this->Event_PlayerAction.Invoke(*this->userTravelMapAction);
	}
	public: void DoAction()
	{
		if (this->userTravelMapAction == nullptr)
		{
			return;
		}

		this->userTravelMapAction->SetAction(this->userTravelMapAction->ActionPoint() - 1);
		if (this->userTravelMapAction->ActionPoint() > 0)
		{
			return;
		}
	}

	private: GameStateController() = default;

	public: void Init(std::vector<std::shared_ptr<Player>> players)
	{
		this->players = players;
	}
	};
}
#endif // !GameStateController_H
