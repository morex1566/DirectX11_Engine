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

void WConsole::Render()
{
	// Create your GUI here
	ImGui::Begin("Tools");

	ImGui::End();
}