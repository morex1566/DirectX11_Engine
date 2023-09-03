#include "pch.h"
#include "Application.h"

int WINAPI wWinMain(HINSTANCE hInstance_, HINSTANCE, PWSTR pCmdLine_, int nCmdShow_)
{
	Application& app = Application::GetInstance();

	app.Initialize(hInstance_);

	while(!app.IsShutdowned())
	{
		app.Update();
	}

	app.Destroy();

	return 0;
}
