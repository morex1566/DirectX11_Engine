#include "pch.h"

#include "Application.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	// CAUTION : Do not change instancing order
	Logger& logger = Logger::Get();

	Application& app = Application::Get();
	{
		app._HInstance = hInstance;
	}

	while(!app.IsExited())
	{
		app.Update();
	}
}