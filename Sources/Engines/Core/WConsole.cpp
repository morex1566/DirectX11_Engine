#include "PCH.h"
#include "WConsole.h"

WConsole::WConsole()
	: OWidget()
{
}

WConsole::~WConsole()
{
	Shutdown();
}

void WConsole::Tick()
{
	OWidget::Tick();

	if (OInput::GetKeyDown(VK_OEM_3))
	{
		bIsConsoleOpened = bIsConsoleOpened ? 0 : 1;
	}
}

void WConsole::Render()
{
	// Create your GUI here
	if (bIsConsoleOpened)
	{
		ImGui::Begin("Tools");

		ImGui::End();
	}
}