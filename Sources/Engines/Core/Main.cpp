#pragma warning (disable : 28251)

#include "PCH.h"
#include "System.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	// 설정파일을 불러옵니다.
	std::unique_ptr<Config> config = std::make_unique<Config>(Utls::ToWString(SOLUTION_GETFILE("Config.json")));
	{
		config->Init();
	}

	// Log를 출력하는 콘솔 창을 생성합니다.
	std::unique_ptr<Console> console = std::make_unique<Console>();
	{
		console->Init();
	}
	
	// 게임 애플리케이션을 시작합니다.
	std::unique_ptr<System> system = std::make_unique<System>(hInstance);
	{
		system->Init();

		system->Start();
		while (system->GetIsLooping())
		{
			system->Update();
		}
	}

	system->Shutdown();
	console->Shutdown();
	config->Shutdown();

	return 0;
}