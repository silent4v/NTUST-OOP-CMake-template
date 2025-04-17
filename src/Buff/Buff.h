#ifndef Buff_H
#define Buff_H

#include <string>

#include "../Effect/IEffect.h"

namespace FTK
{
	enum class EmBuff
	{
		Dizziness,
		Angry,
		Poisoned,
		SpeedUp,
	};

	class Creature;
	class IBuff : public IEffect
	{
	protected: std::weak_ptr<Creature> master;

	public: const int Duration;
	public: const EmBuff Type;
	public: const std::string Notation;
	protected: IBuff(std::shared_ptr<Creature> master, int duration, EmBuff type, const std::string& notation) : master(master), Duration(duration), Type(type), Notation(notation)
	{
		currentDuration = duration;
	}

	private: int currentDuration = 0;
	public: int CurrentDuration() { return currentDuration; }
	public: void ReduceDuration(int value = 1) { currentDuration -= value; }
	};

	namespace buff
	{
		class Dizziness : public IBuff
		{
		public: Dizziness(std::shared_ptr<Creature> master, int duration = 1) : IBuff(master, duration, EmBuff::Dizziness, "D") {}
		public: virtual void Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> effectData) override {}
		};

		class Angry : public IBuff
		{
		public: Angry(std::shared_ptr<Creature> master, int duration = 1) : IBuff(master, duration, EmBuff::Angry, "A") {}
		public: virtual void Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> effectData) override;
		};

		class Poisoned : public IBuff
		{
		public: Poisoned(std::shared_ptr<Creature> master, int duration = 1) : IBuff(master, duration, EmBuff::Poisoned, "P") {}
		public: virtual void Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> effectData) override;
		};

		class SpeedUp : public IBuff
		{
		public: SpeedUp(std::shared_ptr<Creature> master, int duration = 1) : IBuff(master, duration, EmBuff::SpeedUp, "S") {}
		public: virtual void Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> effectData) override;
		};
	}
}
#endif // Buff_H