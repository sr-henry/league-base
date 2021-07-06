#include "league-base/hack.h"

Hack hack;

int main()
{
	std::cout << "Hello World\n";

	while (!GetAsyncKeyState(VK_HOME))
	{
		if (hack.IsGameRunning())
		{
			hack.Update();

		}
	}
	
	return 0;
}