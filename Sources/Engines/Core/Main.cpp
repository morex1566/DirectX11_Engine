#pragma warning (disable : 28251)

#include "PCH.h"
#include "System.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	// 설정파일을 불러옵니다.
	std::unique_ptr<Config> config = std::make_unique<Config>(SOLUTION_FILE_PATH("Config.json"));
	{
		config->Init();
	}

	// Log를 출력하는 콘솔 창을 생성합니다.
	std::unique_ptr<Console> console = std::make_unique<Console>();
	{
		console->Init();
	}

	// DirectXTex를 사용하기 위해, 초기화합니다.
	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
	{
		Console::LogError(L"DirectXTex, initialize() is failed.", __FILE__, __LINE__);
		return 0;
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