#include "pch.h"
#include "Application.h"
#include "D3DManager.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	Application& app = Application::Get();
	{
		app.Initialize(hInstance);
	}

	if(!app.IsStarted())
	{
		app.Start();

		while (!app.IsExited())
		{
			app.Update();
		}
	}
}