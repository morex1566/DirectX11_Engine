#include "PCH.h"
#include "OApplication.h"
#include "OConsole.h"

int WINAPI WinMain(HINSTANCE HInstance, HINSTANCE HPrevInstance, PSTR Scmdline, int Cmdshow)
{
	// Create logger.
	OConsole& Console = OConsole::GetInstance();
	{
		if (Console.Initialize() != Object::EHandleResultType::Success)
		{
			return 0;
		}
	}

	// Create engine application.
	OApplication& Application = OApplication::GetInstance();
	{
		if (Application.Initialize() != Object::EHandleResultType::Success)
		{
			return 0;
		}
	}

	// Application's main flow.
	{
		Application.Start();

		while (Application.GetIsPlaying())
		{
			// Handle window msg.
			MSG Msg;
			while(::PeekMessage(&Msg, nullptr, 0U, 0U, PM_REMOVE))
			{
				::TranslateMessage(&Msg);
				::DispatchMessage(&Msg);
			}

			Application.Tick();
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