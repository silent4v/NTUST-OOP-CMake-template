#ifndef WorldEvent_H
#define WorldEvent_H

#include "../Util/RandomSample.hpp"
#include "Entity.h"
#include "Player.hpp"

namespace FTK
{
	class WorldEvent : public Entity
	{
	protected: const int rollAmount = 4;
	public: inline int RollAmount() const { return this->rollAmount; }
	public: std::string Description(int roll) const
	{
		switch (roll)
		{
		case 0:
			return "Lose (10% * MaxHP) of HP.";
		case 1:
			return "Get 2 dollars.";
		case 2:
			return "Get 4 dollars.";
		case 3:
			return "Get 6 dollars.";
		case 4:
			return "Get 8 dollars.";
		default:
			return "Out of RollAmount";
		}
	}
	public: WorldEvent(const Vector2D& position)
		: Entity("Random event",
			UI::Pixel{ L'?', UI::PixelColor(Util::ForeGroundColor(EmColor::White)) },
			EmType::WorldEvent,
			position)

	{
	}

	public: double Probability(std::shared_ptr<Player> player) const
	{
		return .5;
	}
	public: int Roll(std::shared_ptr<Player> player, int focus) const
	{
		double successProbability = this->Probability(player);
		std::vector<double> probabilities{
			1. - successProbability,
			successProbability
		};

		int successfulCount = focus;
		if (focus >= rollAmount)
		{
			successfulCount = rollAmount;
			return successfulCount;
		}

		for (int i = 0; i < rollAmount - focus; i++)
		{
			int isSuccessful = Util::RandomSample(probabilities, rand(), 2);
			if (isSuccessful == 1)
			{
				successfulCount++;
			}
		}

		return successfulCount;
	}
	public: void Happen(std::shared_ptr<Player> player, int roll);

	};
}
#endif // !WorldEvent_H