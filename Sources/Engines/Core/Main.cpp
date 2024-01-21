#pragma warning (disable : 28251)

#include "PCH.h"
#include "System.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	std::unique_ptr<Config> config = std::make_unique<Config>(Utls::ToWString(SOLUTION_GETFILE("Config.json")));
	{
		config->Init();
		Console::Log(L"Init Config OK.");
	}

	std::unique_ptr<System> system = std::make_unique<System>(hInstance);

	system->Init();
	system->Start();
	while (system->GetIsLooping())
	{
		system->Update();
	}

	system->Shutdown();

	return 0;
}