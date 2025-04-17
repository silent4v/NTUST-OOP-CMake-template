#include "UserEvent.h"

#include <conio.h>

namespace FTK
{
	std::optional<EmKeyBoard> FTK::UserEvent::CheckKeyBoardEvent() const
	{
		if (!_kbhit())
			return std::nullopt;

		EmKeyBoard result = EmKeyBoard::InValid;

		int key = _getch();
		// system
		if (key == 0xE0 || key == 0x00)
		{
			key = _getch();
			if (key == 0x00)
			{
				if (59 <= key && key <= 68)
				{
					result = EmKeyBoard(int(EmKeyBoard::F1) + key - 59);
				}
				else
				{
					result = EmKeyBoard::InValid;
				}
			}
			else
			{
				switch (key)
				{
				case 77:
					result = EmKeyBoard::RightArrow;
					break;
				case 75:
					result = EmKeyBoard::LeftArrow;
					break;
				case 72:
					result = EmKeyBoard::UpArrow;
					break;
				case 80:
					result = EmKeyBoard::DownArrow;
					break;
				default:
					result = EmKeyBoard::InValid;
					break;
				}
			}
		}
		else
		{
			// upper case ABC
			if (65 <= key && key <= 90)
			{
				result = EmKeyBoard(int(EmKeyBoard::A) + key - 65);
			}
			else if (97 <= key && key <= 122) // lower case abc
			{
				result = EmKeyBoard(int(EmKeyBoard::A) + key - 97);
			}
			else if (48 <= key && key <= 57) // 0~9
			{
				result = EmKeyBoard(int(EmKeyBoard::_0) + key - 48);
			}
			else
			{
				switch (key)
				{
				case 8:
					result = EmKeyBoard::BackSpace;
					break;
				case 9:
					result = EmKeyBoard::Tab;
					break;
				case 13:
					result = EmKeyBoard::Enter;
					break;
				default:
					result = EmKeyBoard::InValid;
					break;
				}
			}
		}

		// Invoke
		this->Event_KeyBoard.Invoke(result);

		return result;
	}
}