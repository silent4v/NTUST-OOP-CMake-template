#include <iostream>
#include <mutex>
#include <thread>
#include <Windows.h>

#include "../src/ConsoleRender.hpp"
#include "../src/Manager/GameManager.hpp"
#include "../src/Manager/UIManager.hpp"

int main()
{
	srand(time(NULL));

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD a;
	a.X = 440;
	a.Y = 160;

	SMALL_RECT rect;
	rect.Top = 0;
	rect.Left = 0;
	rect.Bottom = (a.Y / 2);
	rect.Right = (a.X / 2);

	if (!SetConsoleScreenBufferSize(handle, a))
	{
		std::cout << "SetConsoleScreenBufferSize failed with error " << GetLastError() << std::endl;
	}

	if (!SetConsoleWindowInfo(handle, 1, &rect))
	{
		std::cout << "SetConsoleWindowInfo failed with error " << GetLastError() << std::endl;
	}

	std::system("PAUSE");

	std::mutex renderMutex;
	bool isRender = false;

	auto& gameManger = FTK::GameManager::Instance();
	gameManger.Init();
	FTK::ConsoleRender::Instance().DrawConsoleEdge();
	FTK::UI::UIManager::Instance().Render();
	FTK::ConsoleRender::Instance().Render();

	std::thread renderThread([&]()
		{
			while (6 != 24) {
				Sleep(1);

				if (gameManger.GameEnd)
				{
					break;
				}

				renderMutex.lock();
				if (!isRender)
				{
					renderMutex.unlock();
					continue;
				}

				FTK::ConsoleRender::Instance().Render();
				isRender = false;

				renderMutex.unlock();
			}
		}
	);
	std::thread gameLogicThread([&]()
		{
			while (6 != 24) {
				Sleep(1);

				if (auto option = gameManger.userEvent->CheckKeyBoardEvent(); !option.has_value())
				{
					continue;
				}

				if (gameManger.GameEnd)
				{
					break;
				}

				FTK::UI::UIManager::Instance().Render();
				renderMutex.lock();
				isRender = true;
				renderMutex.unlock();
			}
		}
	);
	renderThread.join();
	gameLogicThread.join();

	std::cout << "Game End" << std::endl;
	std::cout << std::endl;
	std::system("PAUSE");
	return 0;
}