#ifndef UserEvent_H

#define UserEvent_H

#include <functional>
#include <memory>
#include <optional>

#include "../Define/KeyBoard.h"
#include "../Map/Tile.h"
#include "../Util/Event.hpp"
#include "../Util/Singleton.hpp"

namespace FTK
{
	class UserEvent : public Util::Singleton<UserEvent>
	{
	private: friend class Util::Singleton<UserEvent>;

#pragma region Manager

#pragma endregion

	private: UserEvent() {};
	private: void init()
	{

	}
	public: void Init()
	{
		this->init();
	}

	public: Util::Event<void, EmKeyBoard> Event_KeyBoard;
	public: std::optional<EmKeyBoard> CheckKeyBoardEvent() const;
	};
}
#endif // !UserEvent_H
