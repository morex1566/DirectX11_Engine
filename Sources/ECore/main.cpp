#include "PCH.h"
#include "SApplication.h"
#include "SConsole.h"
#include "SReflection.h"

int WINAPI WinMain(HINSTANCE HInstance, HINSTANCE HPrevInstance, PSTR Scmdline, int Cmdshow)
{
	// Create logger.
	SConsole& Console = SConsole::GetInstance();
	{
		Console.Initialize();
	}

	// Create Reflection.
	SReflection& Reflection = SReflection::GetInstance();
	{
		Reflection.Initialize();
	}

	// Create engine application.
	SApplication& Application = SApplication::GetInstance();
	{
		Application.Initialize(HInstance);
	}

	// Application's main flow.
	{
		Application.Start();

		while (SApplication::CheckIsPlaying())
		{
			// Handle window msg.
			MSG Msg;
			while(::PeekMessage(&Msg, nullptr, 0U, 0U, PM_REMOVE))
			{
				::TranslateMessage(&Msg);
				::DispatchMessage(&Msg);
			}

			Application.Tick();

			Application.Draw();
		}

		Application.End();
	}

	// Release instance.
	{
		Application.Release();
		Console.Release();
	}

	return 0;
}