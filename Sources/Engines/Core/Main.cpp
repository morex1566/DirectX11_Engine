#pragma warning (disable : 28251)

#include "PCH.h"
#include "Windows.h"
#include "System.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	std::unique_ptr<System> system = std::make_unique<System>(hInstance);

	system->Init();
	system->Start();
	while (system->IsLooping())
	{
		system->Update();
	}

	system->Shutdown();

	return 0;
}