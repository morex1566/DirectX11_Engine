// ReSharper disable CppLocalVariableMayBeConst

#include "pch.h"
#include "Application.h"
#include "D3DManager.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	HRESULT result = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if(FAILED(result))
	{
		return 0;
	}

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

	app.Shutdown();

	return 0;
}